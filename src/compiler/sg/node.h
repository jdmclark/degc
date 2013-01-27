#pragma once

#include "compiler/ast/node.h"

#define SGVISITOR_ACCEPT_ABSTRACT								\
	public:														\
	virtual void Accept(Deg::Compiler::SG::Visitor& v) = 0;		\
	private:

#define SGVISITOR_ACCEPT										\
	public:														\
	virtual void Accept(Deg::Compiler::SG::Visitor& v);			\
	private:

namespace Deg {
namespace Compiler {
namespace SG {

class Visitor;

class Node {
	SGVISITOR_ACCEPT_ABSTRACT
public:
	virtual ~Node();
};

// Symbols

class Symbol : public Node {
	SGVISITOR_ACCEPT_ABSTRACT
};

class NumberSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class QuantitySymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class BooleanSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class SetSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class ProgramSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Program* ast_program;
};

class RecordSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Record* ast_record;
};

class FunctionSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Function* ast_function;
};

class EnumerationSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Enumeration* ast_enumeration;
};

class ErrorSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

}
}
}
