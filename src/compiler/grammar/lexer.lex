%option reentrant
%option never-interactive
%option bison-bridge
%option bison-locations
%option yylineno
%option noyywrap
%option nodefault
%option prefix="gra_"
%option stack

%{
	#include "instance.h"
	#include "parser.hpp"
	
	#include <string.h>
	#include <stdlib.h>
	#include <sstream>
	#include <boost/format.hpp>
	
	using namespace Deg::Compiler;	
	#define YY_EXTRA_TYPE Grammar::Instance*
	
	#define YY_USER_ACTION {											\
		yylloc->filename = yyextra->Filename;							\
		if(yylloc->first_line != (unsigned int)yylineno) yycolumn = 1;	\
		yylloc->first_line = yylloc->last_line = yylineno;				\
		yylloc->first_column = yycolumn;								\
		yylloc->last_column = yycolumn + yyleng - 1;					\
		yycolumn += yyleng;												\
	}
	
	#define YY_INPUT(buf, result, max_size) {							\
		buf[0] = yyextra->GetNext();									\
		if(buf[0] == '\0') result = YY_NULL;							\
		else result = 1;												\
	}
%}

D			[0-9]
L			[_a-zA-Z]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+

%x LINE_COMMENT
%x NORMAL
%s INDENTATION

%%

<LINE_COMMENT>{
	"\n"			{ unput(*yytext); yy_pop_state(yyextra->GetScanner()); }
	<<EOF>>			{ yy_pop_state(yyextra->GetScanner()); }
	.				/* eat non-terminal input */
}

<INITIAL,INDENTATION>{
	"#"				{ yy_push_state(LINE_COMMENT, yyextra->GetScanner()); }
	"    "			{ ++yyextra->CurrentLineIndentation; }
	"\n"			{ yyextra->CurrentLineIndentation = 0; }
	[[:space:]]		/* Ignore other whitespace */
	<<EOF>>			{
						if(yyextra->GetIndentationLevel() > 0) {
							yyextra->PopIndentation();
							return DEDENT;
						}
						else {
							yyterminate();
						}
					}
	.				{
						unput(*yytext);
						if(yyextra->CurrentLineIndentation > yyextra->GetIndentationLevel()) {
							yyextra->PushIndentation();
							return INDENT;
						}
						else if(yyextra->CurrentLineIndentation < yyextra->GetIndentationLevel()) {
							yyextra->PopIndentation();
							return DEDENT;
						}
						else {
							yy_push_state(NORMAL, yyextra->GetScanner());
						}
					}
}

<NORMAL>{
	"#"				{ yy_push_state(LINE_COMMENT, yyextra->GetScanner()); }
	
	"all"			{ return ALL; }
	"and"			{ return AND; }
	"any"			{ return ANY; }
	"assert"		{ return ASSERT; }
	"as"			{ return AS; }
	"best"			{ return BEST; }
	"by"			{ return BY; }
	"either"		{ return EITHER; }
	"else"			{ return ELSE; }
	"embed"			{ return EMBED; }
	"enum"			{ return ENUM; }
	"exists"		{ return EXISTS; }
	"extends"		{ return EXTENDS; }
	"for"			{ return FOR; }
	"from"			{ return FROM; }
	"function"		{ return FUNCTION; }
	"if"			{ return IF; }
	"import"		{ return IMPORT; }
	"intersect"		{ return INTERSECT; }
	"in"			{ return IN; }
	"limit"			{ return LIMIT; }
	"module"		{ return MODULE; }
	"not"			{ return NOT; }
	"or"			{ return OR; }
	"panic"			{ return PANIC; }
	"program"		{ return PROGRAM; }
	"record"		{ return RECORD; }
	"setminus"		{ return SETMINUS; }
	"set"			{ return SET; }
	"take"			{ return TAKE; }
	"union"			{ return UNION; }
	"with"			{ return WITH; }
	
	"true"			{ yylval->boolean = true; return BOOLEAN_LITERAL; }
	"false"			{ yylval->boolean = false; return BOOLEAN_LITERAL; }
	
	{L}({L}|{D})*	{ yylval->string = strdup(yytext); return IDENTIFIER; }
	
	{D}+			{ yylval->string = strdup(yytext); return NUMERIC_LITERAL; }
	{D}+"."{D}+		{ yylval->string = strdup(yytext); return NUMERIC_LITERAL; }
	
	"@"				{ return '@'; }
	
	":"				{ return ':'; }
	
	","				{ return ','; }
	"."				{ return '.'; }
	
	"->"			{ return MAPS_TO; }
	
	"+"				{ return '+'; }
	"-"				{ return '-'; }
	"*"				{ return '*'; }
	"/"				{ return '/'; }
	
	"<"				{ return '<'; }
	"<="			{ return LE_OP; }
	">"				{ return '>'; }
	">="			{ return GE_OP; }
	"="				{ return '='; }
	"!="			{ return NE_OP; }
	
	"("				{ yyextra->PushParentheses(); return '('; }
	")"				{ yyextra->PopParentheses(); return ')'; }
	"["				{ yyextra->PushParentheses(); return '['; }
	"]"				{ yyextra->PopParentheses(); return ']'; }
	"{"				{ yyextra->PushParentheses(); return '{'; }
	"}"				{ yyextra->PopParentheses(); return '}'; }
	"|"				{ return '|'; }
	
	"\n"			{
						if(!yyextra->IsInsideParentheses()) {
							yyextra->CurrentLineIndentation = 0;
							yy_pop_state(yyextra->GetScanner());
							return ENDLN;
						}
					}
					
	[[:space:]]		/* Ignore whitespace */
	
	<<EOF>>			{ yy_pop_state(yyextra->GetScanner()); return ENDLN; }
	
	.				{
						yyextra->Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::UnrecognizedInput, Diagnostics::ErrorLevel::Error,
							"Scanner", boost::str(boost::format("unrecognized input \'%s\'") % yytext),
							Diagnostics::ErrorLocation(yyextra->Filename, yylloc->first_line, yylloc->first_column, yylloc->last_line, yylloc->last_column)));
					}
}

%%

void Grammar::Instance::InitScanner() {
	yylex_init(&scanner);
	yyset_extra(this, scanner);
}

void Grammar::Instance::DestroyScanner() {
	yylex_destroy(scanner);
}
