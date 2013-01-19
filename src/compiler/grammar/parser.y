%defines
%locations
%pure-parser
%name-prefix="gra_"
%error-verbose
%verbose
%lex-param { void* scanner }
%parse-param { Deg::Compiler::Grammar::Instance* instance }

%expect 0
%expect-rr 0

%code top {
	#include "instance.h"	
	#define YYLTYPE Deg::Compiler::Diagnostics::ErrorLocation
}

%code provides {
	#include "compiler/diagnostics/errorlocation.h"
	#define YYLTYPE Deg::Compiler::Diagnostics::ErrorLocation
}

%union
{
	char* string;
	bool boolean;
	
	Deg::Compiler::AST::TranslationUnit* translation_unit;
}

/* Keywords */
%token ALL AND ANY ASSERT AS BEST BY ELSE EMBED ENUM EXTENDS FOR FROM FUNCTION IF IMPORT IN MODULE NOT OR PANIC PROGRAM RECORD TAKE

/* Punctuators */
%token INDENT DEDENT ENDLN NE_OP GE_OP LE_OP

/* Literals */
%token <string> IDENTIFIER NUMERIC_LITERAL
%token <boolean> BOOLEAN_LITERAL

/* AST nodes */
%type <translation_unit> translation_unit

/* Destructors */
%destructor { free($$); } <string>

%start translation_unit

%{
	using namespace Deg::Compiler;
	
	int yylex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);
		
	void yyerror(YYLTYPE* llocp, Deg::Compiler::Grammar::Instance* instance, const char* err) {
		instance->Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SyntaxError,
			Diagnostics::ErrorLevel::Error, "Parser", err, *llocp));
	}
	
	#define scanner instance->GetScanner()
	#define ast instance->Factory
%}

%%

/********** Expressions **********/

argument_expression_list
	: ')'
	| expression ')'
	| expression ',' argument_expression_list
	;

literal_expression
	: NUMERIC_LITERAL
	| BOOLEAN_LITERAL
	;
	
set_expression
	: '{' '}'
	| '{' IDENTIFIER '}'
	| '{' IDENTIFIER IDENTIFIER '}'
	| '{' IDENTIFIER IDENTIFIER '|' expression '}'
	;
	
primary_expression
	: PANIC
	| literal_expression
	| set_expression
	| IDENTIFIER
	| '(' expression ')'
	;
	
postfix_expression
	: primary_expression
	| postfix_expression '(' argument_expression_list
	| postfix_expression '.' IDENTIFIER
	;
	
unary_operator
	: '-'
	| NOT
	;
	
unary_expression
	: postfix_expression
	| unary_operator postfix_expression
	;
	
multiplicative_operator
	: '*'
	| '/'
	;
	
multiplicative_expression
	: unary_expression
	| multiplicative_expression multiplicative_operator unary_expression
	;
	
additive_operator
	: '+'
	| '-'
	;
	
additive_expression
	: multiplicative_expression
	| additive_expression additive_operator multiplicative_expression
	;
	
relational_operator
	: GE_OP
	| '>'
	| LE_OP
	| '<'
	;
	
relational_expression
	: additive_expression
	| relational_expression relational_operator additive_expression
	;
	
equality_operator
	: '='
	| NE_OP
	;
	
equality_expression
	: relational_expression
	| equality_expression equality_operator relational_expression
	;
	
and_expression
	: equality_expression
	| and_expression AND equality_expression
	;
	
or_expression
	: and_expression
	| or_expression OR and_expression

expression
	: or_expression
	;

/********** Program Members **********/

assert_statement
	: ASSERT expression ENDLN
	;
	
embed_statement
	: EMBED expression ENDLN
	| EMBED ':' ENDLN program_statement_part
	;
	
iteration_statement
	: FOR ALL IDENTIFIER IDENTIFIER ':' ENDLN program_statement_part
	| FOR ALL IDENTIFIER IDENTIFIER IN expression ':' ENDLN program_statement_part
	;
	
selection_statement
	: FOR ANY IDENTIFIER IDENTIFIER ':' ENDLN program_statement_part
	| FOR ANY IDENTIFIER IDENTIFIER IN expression ':' ENDLN program_statement_part
	| FOR BEST IDENTIFIER IDENTIFIER BY IDENTIFIER ':' ENDLN program_statement_part
	| FOR BEST IDENTIFIER IDENTIFIER BY IDENTIFIER IN expression ':' ENDLN program_statement_part
	;
	
branch_statement
	: IF expression ':' ENDLN program_statement_part
	| IF expression ':' ENDLN program_statement_part ELSE branch_statement
	| IF expression ':' ENDLN program_statement_part ELSE ':' ENDLN program_statement_part
	;
	
take_statement
	: TAKE NUMERIC_LITERAL IN expression ENDLN
	;

/********** Program **********/

program_statement
	: assert_statement
	| embed_statement
	| iteration_statement
	| selection_statement
	| branch_statement
	| take_statement
	;

named_program_statement
	: '@' IDENTIFIER program_statement
	| program_statement
	;

program_statement_seq
	: program_statement_seq named_program_statement
	| named_program_statement
	;

program_statement_part
	: INDENT program_statement_seq DEDENT
	| /* Blank */
	;

program
	: PROGRAM IDENTIFIER ':' ENDLN program_statement_part
	| PROGRAM IDENTIFIER EXTENDS IDENTIFIER ':' ENDLN program_statement_part
	;
	
/********** Function **********/

function_selection_expression
	: IF expression ':' ENDLN function_expression ELSE function_selection_expression
	| IF expression ':' ENDLN function_expression ELSE ':' ENDLN function_expression
	;

function_expression
	: INDENT expression ENDLN DEDENT
	| INDENT function_selection_expression DEDENT
	;

function_argument
	: IDENTIFIER IDENTIFIER
	;

function_argument_list
	: ')'
	| function_argument ')'
	| function_argument ',' function_argument_list
	;

function
	: FUNCTION IDENTIFIER '(' function_argument_list ':' ENDLN function_expression
	;

/********** Record **********/

record_member
	: IDENTIFIER IDENTIFIER ENDLN
	;

record_member_seq
	: record_member_seq record_member
	| record_member
	;

record_member_part
	: INDENT record_member_seq DEDENT
	| /* Blank */
	;

record
	: RECORD IDENTIFIER ':' ENDLN record_member_part
	;

/********** Enumeration **********/

enumeration_member
	: IDENTIFIER ENDLN
	;

enumeration_member_seq
	: enumeration_member_seq enumeration_member
	| enumeration_member
	;

enumeration_member_part
	: INDENT enumeration_member_seq DEDENT
	| /* Blank */
	;

enumeration
	: ENUM IDENTIFIER ':' ENDLN enumeration_member_part
	;

/********** Translation Unit **********/

module_name
	: IDENTIFIER
	| module_name '.' IDENTIFIER
	;

module_declaration
	: MODULE module_name ENDLN
	;
	
symbol_import_declaration
	: IDENTIFIER
	| IDENTIFIER AS IDENTIFIER
	;
	
symbol_import_declaration_list
	: ')'
	| symbol_import_declaration ')'
	| symbol_import_declaration ',' symbol_import_declaration_list
	;
	
module_import_declaration
	: FROM module_name IMPORT symbol_import_declaration ENDLN
	| FROM module_name IMPORT '(' symbol_import_declaration_list ENDLN
	;
	
module_import_declaration_list
	: module_import_declaration
	| module_import_declaration_list module_import_declaration
	;
	
import_declaration_part
	: module_import_declaration_list
	| /* Blank */
	;
	
global_declaration
	: program
	| function
	| record
	| enumeration
	;
	
global_declaration_seq
	: global_declaration_seq global_declaration
	| global_declaration
	;
	
global_declaration_part
	: global_declaration_seq
	| /* Blank */
	;

translation_unit
	: module_declaration import_declaration_part global_declaration_part { instance->SetReturnValue($$ = ast->MakeTranslationUnit(@$)); }
	;
