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

%code requires {
	#include "compiler/diagnostics/errorlocation.h"
	#define YYLTYPE YYLTYPE
	typedef Deg::Compiler::Diagnostics::ErrorLocation YYLTYPE;
	
	#define YYLLOC_DEFAULT(Cur, Rhs, N)                        \
     do                                                        \
       if (N)                                                  \
         {                                                     \
           (Cur).first_line   = YYRHSLOC(Rhs, 1).first_line;   \
           (Cur).first_column = YYRHSLOC(Rhs, 1).first_column; \
           (Cur).last_line    = YYRHSLOC(Rhs, N).last_line;    \
           (Cur).last_column  = YYRHSLOC(Rhs, N).last_column;  \
           (Cur).filename     = YYRHSLOC(Rhs, N).filename;     \
         }                                                     \
       else                                                    \
         {                                                     \
           (Cur).first_line   = (Cur).last_line   =            \
             YYRHSLOC(Rhs, 0).last_line;                       \
           (Cur).first_column = (Cur).last_column =            \
             YYRHSLOC(Rhs, 0).last_column;                     \
           (Cur).filename = YYRHSLOC(Rhs, 0).filename;         \
         }                                                     \
     while (0)
}

%code top {
	#include "instance.h"
}

%union
{
	char* string;
	bool boolean;
	
	Deg::Compiler::AST::Typename* type_name;
	std::vector<Deg::Compiler::AST::Typename*>* type_name_list;
	Deg::Compiler::AST::UnaryOperator unary_operator;
	Deg::Compiler::AST::InfixOperator infix_operator;
	Deg::Compiler::AST::Expression* expression;
	std::vector<Deg::Compiler::AST::Expression*>* expression_list;
	Deg::Compiler::AST::Statement* statement;
	std::vector<Deg::Compiler::AST::Statement*>* statement_list;
	Deg::Compiler::AST::FunctionArgument* function_argument;
	std::vector<Deg::Compiler::AST::FunctionArgument*>* function_argument_list;
	Deg::Compiler::AST::RecordMember* record_member;
	std::vector<Deg::Compiler::AST::RecordMember*>* record_member_list;
	Deg::Compiler::AST::EnumerationMember* enumeration_member;
	std::vector<Deg::Compiler::AST::EnumerationMember*>* enumeration_member_list;
	Deg::Compiler::AST::ModuleName* module_name;
	Deg::Compiler::AST::SymbolImport* symbol_import;
	std::vector<Deg::Compiler::AST::SymbolImport*>* symbol_import_list;
	Deg::Compiler::AST::ModuleImport* module_import;
	std::vector<Deg::Compiler::AST::ModuleImport*>* module_import_list;
	Deg::Compiler::AST::Declaration* declaration;
	std::vector<Deg::Compiler::AST::Declaration*>* declaration_list;
	Deg::Compiler::AST::TranslationUnit* translation_unit;
}

/* Keywords */
%token ALL AND ANY ASSERT AS BEST BY EITHER ELSE EMBED ENUM EXISTS EXTENDS FOR FROM
%token FUNCTION IF IMPORT INTERSECT IN LIMIT MODULE NOT OR PANIC PROGRAM
%token RECORD SETMINUS SET TAKE UNION

/* Punctuators */
%token INDENT DEDENT ENDLN MAPS_TO NE_OP GE_OP LE_OP

/* Literals */
%token <string> IDENTIFIER NUMERIC_LITERAL
%token <boolean> BOOLEAN_LITERAL

/* AST nodes */
%type <type_name> typename
%type <type_name_list> function_typename_argument_list
%type <expression_list> argument_expression_list
%type <unary_operator> unary_operator
%type <infix_operator> multiplicative_operator additive_operator relational_operator equality_operator
%type <expression> literal_expression set_expression primary_expression postfix_expression unary_expression
%type <expression> multiplicative_expression additive_expression relational_expression equality_expression
%type <expression> and_expression or_expression expression function_selection_expression function_expression
%type <statement_list> program_statement_seq disjunction_statement_part_seq
%type <statement> named_statement assert_statement embed_statement disjunction_statement disjunction_statement_part iteration_statement
%type <statement> selection_statement branch_statement take_statement program_statement program_statement_part
%type <function_argument> function_argument
%type <function_argument_list> function_argument_list
%type <record_member> record_member
%type <record_member_list> record_member_seq record_member_part
%type <enumeration_member> enumeration_member
%type <enumeration_member_list> enumeration_member_seq enumeration_member_part
%type <declaration> program function record enumeration global_declaration
%type <module_name> module_name module_declaration
%type <symbol_import> symbol_import
%type <symbol_import_list> symbol_import_list
%type <module_import> module_import
%type <module_import_list> module_import_list import_declaration_part
%type <declaration_list> global_declaration_seq global_declaration_part
%type <translation_unit> translation_unit

/* Destructors */
%destructor { free($$); } <string>

%start translation_unit

%{
	using namespace Deg::Compiler;
	using namespace Deg::Compiler::AST;
	
	int yylex(YYSTYPE* lvalp, Deg::Compiler::Diagnostics::ErrorLocation* llocp, void* scanner);
		
	void yyerror(Deg::Compiler::Diagnostics::ErrorLocation* llocp, Deg::Compiler::Grammar::Instance* instance, const char* err) {
		instance->Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SyntaxError,
			Diagnostics::ErrorLevel::Error, "Parser", err, *llocp));
	}
	
	#define scanner instance->GetScanner()
	#define ast instance->Factory
%}

%%

/********** Typenames **********/

function_typename_argument_list
	: ')'
		{ $$ = ast->MakeList<Typename>(); }
	| typename ')'
		{ $$ = ast->MakeList<Typename>($1); }
	| typename ',' function_typename_argument_list
		{
			($3)->insert(($3)->begin(), $1);
			$$ = $3;
		}
	;

typename
	: IDENTIFIER
		{ $$ = ast->MakeNamedTypename($1, @$); }
	| SET '(' IDENTIFIER ')'
		{ $$ = ast->MakeSetTypename($3, @$); }
	| FUNCTION '(' function_typename_argument_list MAPS_TO typename
		{ $$ = ast->MakeFunctionTypename($3, $5, @$); }
	;

/********** Expressions **********/

argument_expression_list
	: ')'
		{ $$ = ast->MakeList<Expression>(); }
	| expression ')'
		{ $$ = ast->MakeList<Expression>($1); }
	| expression ',' argument_expression_list
		{
			($3)->insert(($3)->begin(), $1);
			$$ = $3;
		}
	;

literal_expression
	: NUMERIC_LITERAL
		{ $$ = ast->MakeNumericLiteralExpression($1, @$); }
	| BOOLEAN_LITERAL
		{ $$ = ast->MakeBooleanLiteralExpression($1, @$); }
	;
	
set_expression
	: '{' IDENTIFIER '}'
		{ $$ = ast->MakeTypedSetExpression($2, @$); }
	| '{' IDENTIFIER '|' expression '}'
		{ $$ = ast->MakeConstrainedSetExpression($2, $4, @$); }
	;
	
primary_expression
	: PANIC
		{ $$ = ast->MakePanicExpression(@$); }
	| literal_expression
	| set_expression
	| IDENTIFIER
		{ $$ = ast->MakeIdentifierExpression($1, @$); }
	| '(' expression ')'
		{ $$ = $2; }
	;
	
postfix_expression
	: primary_expression
	| postfix_expression '(' argument_expression_list
		{ $$ = ast->MakeFunctionCallExpression($1, $3, @$); }
	| postfix_expression '.' IDENTIFIER
		{ $$ = ast->MakeMemberAccessExpression($1, $3, @$); }
	;
	
unary_operator
	: '-'
		{ $$ = UnaryOperator::Minus; }
	| NOT
		{ $$ = UnaryOperator::Not; }
	;
	
unary_expression
	: postfix_expression
	| unary_operator postfix_expression
		{ $$ = ast->MakeUnaryExpression($1, $2, @$); }
	| EXISTS unary_expression
		{ $$ = ast->MakeExistsExpression($2, @$); }
	;
	
multiplicative_operator
	: '*'
		{ $$ = InfixOperator::Multiplication; }
	| '/'
		{ $$ = InfixOperator::Division; }
	;
	
multiplicative_expression
	: unary_expression
	| multiplicative_expression multiplicative_operator unary_expression
		{ $$ = ast->MakeInfixExpression($2, $1, $3, @$); }
	;
	
additive_operator
	: '+'
		{ $$ = InfixOperator::Addition; }
	| '-'
		{ $$ = InfixOperator::Subtraction; }
	| SETMINUS
		{ $$ = InfixOperator::SetMinus; }
	;
	
additive_expression
	: multiplicative_expression
	| additive_expression additive_operator multiplicative_expression
		{ $$ = ast->MakeInfixExpression($2, $1, $3, @$); }
	;
	
relational_operator
	: GE_OP
		{ $$ = InfixOperator::GreaterEqual; }
	| '>'
		{ $$ = InfixOperator::Greater; }
	| LE_OP
		{ $$ = InfixOperator::LessEqual; }
	| '<'
		{ $$ = InfixOperator::Less; }
	;
	
relational_expression
	: additive_expression
	| relational_expression relational_operator additive_expression
		{ $$ = ast->MakeInfixExpression($2, $1, $3, @$); }
	;
	
equality_operator
	: '='
		{ $$ = InfixOperator::Equal; }
	| NE_OP
		{ $$ = InfixOperator::NotEqual; }
	;
	
equality_expression
	: relational_expression
	| equality_expression equality_operator relational_expression
		{ $$ = ast->MakeInfixExpression($2, $1, $3, @$); }
	;
	
and_expression
	: equality_expression
	| and_expression AND equality_expression
		{ $$ = ast->MakeInfixExpression(InfixOperator::And, $1, $3, @$); }
	| and_expression INTERSECT equality_expression
		{ $$ = ast->MakeInfixExpression(InfixOperator::Intersect, $1, $3, @$); }
	;
	
or_expression
	: and_expression
	| or_expression OR and_expression
		{ $$ = ast->MakeInfixExpression(InfixOperator::Or, $1, $3, @$); }
	| or_expression UNION and_expression
		{ $$ = ast->MakeInfixExpression(InfixOperator::Union, $1, $3, @$); }
	;

expression
	: or_expression
	;

/********** Program Members **********/

named_statement
	: '@' IDENTIFIER program_statement
		{ $$ = ast->MakeNamedStatement($2, $3, @$); }
	;

assert_statement
	: ASSERT expression ENDLN
		{ $$ = ast->MakeAssertStatement($2, @$); }
	;
	
embed_statement
	: EMBED expression ENDLN
		{ $$ = ast->MakeEmbedStatement($2, @$); }
	| EMBED ':' ENDLN program_statement_part
		{ $$ = ast->MakeEmbedInlineStatement($4, @$); }
	;
	
disjunction_statement_part
	: OR ':' ENDLN program_statement_part
		{ $$ = $4; }
	;
	
disjunction_statement_part_seq
	: disjunction_statement_part
		{ $$ = ast->MakeList<Statement>($1); }
	| disjunction_statement_part_seq disjunction_statement_part
		{ ($1)->push_back($2); $$ = $1; }
	;
	
disjunction_statement
	: EITHER ':' ENDLN program_statement_part disjunction_statement_part_seq
		{ ($5)->insert(($5)->begin(), $4); $$ = ast->MakeDisjunctionStatement($5, @$); }
	;
	
iteration_statement
	: FOR ALL IDENTIFIER IDENTIFIER ':' ENDLN program_statement_part
		{ $$ = ast->MakeForAllStatement($3, $4, ast->MakeTypedSetExpression($3, @5), $7, @$); }
	| FOR ALL IDENTIFIER IDENTIFIER IN expression ':' ENDLN program_statement_part
		{ $$ = ast->MakeForAllStatement($3, $4, $6, $9, @$); }
	;
	
selection_statement
	: FOR ANY IDENTIFIER IDENTIFIER ':' ENDLN program_statement_part
		{ $$ = ast->MakeForAnyStatement($3, $4, ast->MakeTypedSetExpression($3, @5), $7, @$); }
	| FOR ANY IDENTIFIER IDENTIFIER IN expression ':' ENDLN program_statement_part
		{ $$ = ast->MakeForAnyStatement($3, $4, $6, $9, @$); }
	| FOR BEST IDENTIFIER IDENTIFIER BY expression ':' ENDLN program_statement_part
		{ $$ = ast->MakeForBestStatement($3, $4, $6, ast->MakeTypedSetExpression($3, @7), $9, @$); }
	| FOR BEST IDENTIFIER IDENTIFIER BY expression IN expression ':' ENDLN program_statement_part
		{ $$ = ast->MakeForBestStatement($3, $4, $6, $8, $11, @$); }
	;
	
branch_statement
	: IF expression ':' ENDLN program_statement_part
		{ $$ = ast->MakeIfStatement($2, $5, @$); }
	| IF expression ':' ENDLN program_statement_part ELSE branch_statement
		{ $$ = ast->MakeIfElseStatement($2, $5, $7, @$); }
	| IF expression ':' ENDLN program_statement_part ELSE ':' ENDLN program_statement_part
		{ $$ = ast->MakeIfElseStatement($2, $5, $9, @$); }
	;
	
take_statement
	: TAKE expression IN expression ENDLN
		{ $$ = ast->MakeTakeStatement($2, $4, @$); }
	| LIMIT expression IN expression ENDLN
		{ $$ = ast->MakeLimitStatement($2, $4, @$); }
	;

/********** Program **********/

program_statement
	: named_statement
	| assert_statement
	| embed_statement
	| disjunction_statement
	| iteration_statement
	| selection_statement
	| branch_statement
	| take_statement
	;

program_statement_seq
	: program_statement_seq program_statement
		{ ($1)->push_back($2); $$ = $1; }
	| program_statement
		{ $$ = ast->MakeList<Statement>($1); }
	;

program_statement_part
	: INDENT program_statement_seq DEDENT
		{ $$ = ast->MakeCompoundStatement($2, @$); }
	| /* Blank */
		{ $$ = ast->MakeCompoundStatement(ast->MakeList<Statement>(), @$); }
	;

program
	: PROGRAM IDENTIFIER ':' ENDLN program_statement_part
		{ $$ = ast->MakeProgram($2, ast->MakeList<FunctionArgument>(), "", ast->MakeList<Expression>(), $5, @$); }
	| PROGRAM IDENTIFIER '(' function_argument_list ':' ENDLN program_statement_part
		{ $$ = ast->MakeProgram($2, $4, "", ast->MakeList<Expression>(), $7, @$); }
	| PROGRAM IDENTIFIER EXTENDS IDENTIFIER ':' ENDLN program_statement_part
		{ $$ = ast->MakeProgram($2, ast->MakeList<FunctionArgument>(), $4, ast->MakeList<Expression>(), $7, @$); }
	| PROGRAM IDENTIFIER '(' function_argument_list EXTENDS IDENTIFIER ':' ENDLN program_statement_part
		{ $$ = ast->MakeProgram($2, $4, $6, ast->MakeList<Expression>(), $9, @$); }
	| PROGRAM IDENTIFIER EXTENDS IDENTIFIER '(' argument_expression_list ':' ENDLN program_statement_part
		{ $$ = ast->MakeProgram($2, ast->MakeList<FunctionArgument>(), $4, $6, $9, @$); }
	| PROGRAM IDENTIFIER '(' function_argument_list EXTENDS IDENTIFIER '(' argument_expression_list ':' ENDLN program_statement_part
		{ $$ = ast->MakeProgram($2, $4, $6, $8, $11, @$); }
	;
	
/********** Function **********/

function_selection_expression
	: IF expression ':' ENDLN function_expression ELSE function_selection_expression
		{ $$ = ast->MakeFunctionIfElseExpression($2, $5, $7, @$); }
	| IF expression ':' ENDLN function_expression ELSE ':' ENDLN function_expression
		{ $$ = ast->MakeFunctionIfElseExpression($2, $5, $9, @$); }
	;

function_expression
	: INDENT expression ENDLN DEDENT
		{ $$ = $2; }
	| INDENT function_selection_expression DEDENT
		{ $$ = $2; }
	;

function_argument
	: typename IDENTIFIER
		{ $$ = ast->MakeFunctionArgument($1, $2, @$); }
	;

function_argument_list
	: ')'
		{ $$ = ast->MakeList<FunctionArgument>(); }
	| function_argument ')'
		{ $$ = ast->MakeList<FunctionArgument>($1); }
	| function_argument ',' function_argument_list
		{ ($3)->insert(($3)->begin(), $1); $$ = $3; }
	;

function
	: FUNCTION IDENTIFIER '(' function_argument_list MAPS_TO typename ':' ENDLN function_expression
		{ $$ = ast->MakeFunction($2, $4, $6, $9, @$); }
	;

/********** Record **********/

record_member
	: typename IDENTIFIER ENDLN
		{ $$ = ast->MakeRecordMember($1, $2, @$); }
	;

record_member_seq
	: record_member_seq record_member
		{ ($1)->push_back($2); $$ = $1; }
	| record_member
		{ $$ = ast->MakeList<RecordMember>($1); }
	;

record_member_part
	: INDENT record_member_seq DEDENT
		{ $$ = $2; }
	| /* Blank */
		{ $$ = ast->MakeList<RecordMember>(); }
	;

record
	: RECORD IDENTIFIER ':' ENDLN record_member_part
		{ $$ = ast->MakeRecord($2, $5, @$); }
	;

/********** Enumeration **********/

enumeration_member
	: IDENTIFIER ENDLN
		{ $$ = ast->MakeEnumerationMember($1, @$); }
	;

enumeration_member_seq
	: enumeration_member_seq enumeration_member
		{ ($1)->push_back($2); $$ = $1; }
	| enumeration_member
		{ $$ = ast->MakeList<EnumerationMember>($1); }
	;

enumeration_member_part
	: INDENT enumeration_member_seq DEDENT
		{ $$ = $2; }
	| /* Blank */
		{ $$ = ast->MakeList<EnumerationMember>(); }
	;

enumeration
	: ENUM IDENTIFIER ':' ENDLN enumeration_member_part
		{ $$ = ast->MakeEnumeration($2, $5, @$); }
	;

/********** Translation Unit **********/

module_name
	: IDENTIFIER
		{ $$ = ast->MakeModuleName($1, @$); }
	| module_name '.' IDENTIFIER
		{
			($1)->Name.push_back('.');
			($1)->Name.append($3);
			($1)->Location = @$;
			$$ = $1;
		}
	;

module_declaration
	: MODULE module_name ENDLN
		{ $$ = $2; }
	;
	
symbol_import
	: IDENTIFIER
		{ $$ = ast->MakeSymbolImport($1, $1, @$); }
	| IDENTIFIER AS IDENTIFIER
		{ $$ = ast->MakeSymbolImport($1, $3, @$); }
	;
	
symbol_import_list
	: ')'
		{ $$ = ast->MakeList<SymbolImport>(); }
	| symbol_import ')'
		{ $$ = ast->MakeList<SymbolImport>($1); }
	| symbol_import ',' symbol_import_list
		{ ($3)->insert(($3)->begin(), $1); $$ = $3; }
	;
	
module_import
	: FROM module_name IMPORT symbol_import ENDLN
		{ $$ = ast->MakeModuleImport($2, ast->MakeList<SymbolImport>($4), @$); }
	| FROM module_name IMPORT '(' symbol_import_list ENDLN
		{ $$ = ast->MakeModuleImport($2, $5, @$); }
	;
	
module_import_list
	: module_import
		{ $$ = ast->MakeList<ModuleImport>($1); }
	| module_import_list module_import
		{ ($1)->push_back($2); $$ = $1; }
	;
	
import_declaration_part
	: module_import_list
	| /* Blank */
		{ $$ = ast->MakeList<ModuleImport>(); }
	;
	
global_declaration
	: program
	| function
	| record
	| enumeration
	;
	
global_declaration_seq
	: global_declaration_seq global_declaration
		{ ($1)->push_back($2); $$ = $1; }
	| global_declaration
		{ $$ = ast->MakeList<Declaration>($1); }
	;
	
global_declaration_part
	: global_declaration_seq
	| /* Blank */
		{ $$ = ast->MakeList<Declaration>(); }
	;

translation_unit
	: module_declaration import_declaration_part global_declaration_part
		{ instance->SetReturnValue($$ = ast->MakeTranslationUnit($1, $2, $3, @$)); }
	;
