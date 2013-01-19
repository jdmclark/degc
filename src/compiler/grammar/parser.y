%defines
%locations
%pure-parser
%name-prefix="gra_"
%error-verbose
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
	
	Deg::Compiler::AST::TranslationUnit* translation_unit;
}

/* Keywords */
%token AS ENUM FROM IMPORT MODULE RECORD

/* Punctuators */
%token INDENT DEDENT ENDLN

/* Literals */
%token <string> IDENTIFIER

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
	: record
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
