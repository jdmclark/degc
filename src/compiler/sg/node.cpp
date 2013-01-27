#include "node.h"
#include "visitor.h"

Deg::Compiler::SG::Node::~Node() {
	return;
}

#define SGVISITOR_ACCEPT_IMPL(x)						\
	void Deg::Compiler::SG::x::Accept(Visitor& v) {		\
		v.Visit##x(*this);								\
		return;											\
	}

SGVISITOR_ACCEPT_IMPL(ProgramSymbol);
SGVISITOR_ACCEPT_IMPL(EnumerationSymbol);
SGVISITOR_ACCEPT_IMPL(FunctionSymbol);
SGVISITOR_ACCEPT_IMPL(RecordSymbol);
SGVISITOR_ACCEPT_IMPL(ErrorSymbol);
