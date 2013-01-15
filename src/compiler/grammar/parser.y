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
%token AS FROM IMPORT MODULE

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
	
import_declaration
	: FROM module_name IMPORT symbol_import_declaration ENDLN
	| FROM module_name IMPORT '(' symbol_import_declaration_list ENDLN
	;
	
import_declaration_list
	: import_declaration
	| import_declaration_list import_declaration
	;
	
import_declaration_part
	: import_declaration_list
	| /* Blank */
	;

translation_unit
	: module_declaration import_declaration_part { instance->SetReturnValue($$ = ast->MakeTranslationUnit(@$)); }
	;
