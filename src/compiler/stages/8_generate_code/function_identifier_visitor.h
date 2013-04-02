#pragma once

#include "compiler/ir/printer.h"
#include "compiler/sg/visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class FunctionIdentifierVisitor : public SG::Visitor {
private:
	IR::Printer& code;
public:
	FunctionIdentifierVisitor(IR::Printer& code, Diagnostics::Report& report);

	void VisitFunctionSymbol(SG::FunctionSymbol& n);
};

}
}
}
}
