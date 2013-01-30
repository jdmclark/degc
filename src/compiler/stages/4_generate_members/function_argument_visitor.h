#pragma once

#include "compiler/ast/visitor.h"
#include "compiler/sg/node.h"
#include "compiler/sg/module.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class FunctionArgumentVisitor : public AST::Visitor {
private:
	SG::FunctionSymbol& symbol;
	SG::Module& module;

public:
	FunctionArgumentVisitor(SG::FunctionSymbol& symbol, SG::Module& module, Diagnostics::Report& report);

	void VisitFunctionArgument(AST::FunctionArgument& n);
};

}
}
}
}
