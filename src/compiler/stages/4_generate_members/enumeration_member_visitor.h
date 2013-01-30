#pragma once

#include "compiler/ast/visitor.h"
#include "compiler/sg/node.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class EnumerationMemberVisitor : public AST::Visitor {
private:
	SG::EnumerationSymbol& symbol;

public:
	EnumerationMemberVisitor(SG::EnumerationSymbol& symbol, Diagnostics::Report& report);

	void VisitEnumerationMember(AST::EnumerationMember& n);
};

}
}
}
}
