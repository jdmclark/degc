#include "instance.h"
#include <iostream>
#include <exception>

Deg::Compiler::Grammar::Instance::Instance(std::istream& file, const std::string& filename, AST::Factory& ast, Diagnostics::Report& report)
	: file(file), scanner(nullptr), indentationLevel(0), insideParentheses(0), ReturnValue(nullptr),
	  Filename(ast.StoreValue<std::string>(filename)->c_str()), Factory(&ast), Report(report), CurrentLineIndentation(0) {
	InitScanner();
	return;
}

Deg::Compiler::Grammar::Instance::~Instance() {
	DestroyScanner();
}

char Deg::Compiler::Grammar::Instance::GetNext() {
	try {
		char c = file.get();
		if(file.good()) {
			return c;
		}
	}
	catch(const std::exception&) {
		return '\0';
	}

	return '\0';
}

int gra_parse(Deg::Compiler::Grammar::Instance*);

Deg::Compiler::AST::TranslationUnit* Deg::Compiler::Grammar::Instance::Parse() {
	gra_parse(this);
	return ReturnValue;
}
