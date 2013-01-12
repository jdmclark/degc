#pragma once

#include "compiler/diagnostics/errorlocation.h"
#include <string>
#include <vector>

#define ASTVISITOR_ACCEPT_ABSTRACT								\
	public:														\
	virtual void Accept(Deg::Compiler::AST::Visitor& v) = 0;	\
	private:

#define ASTVISITOR_ACCEPT										\
	public:														\
	virtual void Accept(Deg::Compiler::AST::Visitor& v);		\
	private:

namespace Deg {
namespace Compiler {
namespace AST {

class Visitor;

class Node {
	ASTVISITOR_ACCEPT_ABSTRACT
protected:
	Node();

public:
	virtual ~Node();

	Diagnostics::ErrorLocation Location;
};

/* Translation unit */

class TranslationUnit : public Node {
	ASTVISITOR_ACCEPT
};

}
}
}
