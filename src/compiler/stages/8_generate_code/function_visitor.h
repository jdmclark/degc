#pragma once

#include "compiler/ir/printer.h"
#include "compiler/sg/visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class FunctionVisitor : public SG::Visitor {
private:
	size_t arg_ct;
	IR::Printer& code;
public:
	FunctionVisitor(size_t arg_ct, IR::Printer& code, Diagnostics::Report& report);

	void VisitIdentifierExpression(SG::IdentifierExpression& n);
};

}
}
}
}
