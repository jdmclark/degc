#include "node.h"
#include "visitor.h"

Deg::Compiler::AST::Node::Node() {
	return;
}

Deg::Compiler::AST::Node::~Node() {
	return;
}

#define ASTVISITOR_ACCEPT_IMPL(x)						\
	void Deg::Compiler::AST::x::Accept(Visitor& v) {	\
		v.Visit##x(*this);								\
		return;											\
	}

ASTVISITOR_ACCEPT_IMPL(TranslationUnit);
