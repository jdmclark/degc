#pragma once

#include "compiler/ast/visitor.h"
#include "compiler/sg/node.h"
#include "compiler/sg/module.h"
#include <memory>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class TypenameVisitor : public AST::Visitor {
private:
	SG::Module& module;
	bool can_take_quantity;
public:
	TypenameVisitor(SG::Module& module, Diagnostics::Report& report, bool can_take_quantity = false);

	std::unique_ptr<SG::Type> TypenameType;
	bool IsQuantityType;

	void VisitNamedTypename(AST::NamedTypename& n);
	void VisitSetTypename(AST::SetTypename& n);
	void VisitConstrainedSetTypename(AST::ConstrainedSetTypename& n);
	void VisitFunctionTypename(AST::FunctionTypename& n);
};

}
}
}
}
