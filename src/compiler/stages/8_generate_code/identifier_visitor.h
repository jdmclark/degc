#pragma once

#include "compiler/ir/printer.h"
#include "compiler/sg/visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class IdentifierVisitor : public SG::Visitor {
private:
	IR::Printer& code;
public:
	IdentifierVisitor(IR::Printer& code, Diagnostics::Report& report);

	void VisitFunctionArgumentSymbol(SG::FunctionArgumentSymbol& n);
};

}
}
}
}