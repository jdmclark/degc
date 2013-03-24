#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/node.h"
#include "compiler/ir/printer.h"
#include "runtime/math/fixed.h"
#include <memory>
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class ExpressionVisitor : public SG::Visitor {
private:
	IR::Printer& code;

public:
	ExpressionVisitor(IR::Printer& code, Diagnostics::Report& report);

	void VisitNumericExpression(SG::NumericExpression& e);
	void VisitBooleanExpression(SG::BooleanExpression& e);

	void VisitPanicExpression(SG::PanicExpression& e);

	void VisitIdentifierExpression(SG::IdentifierExpression& e);

	void VisitUnaryExpression(SG::UnaryExpression& e);
	void VisitInfixExpression(SG::InfixExpression& e);

	void VisitFunctionIfElseExpression(SG::FunctionIfElseExpression& e);
};

}
}
}
}
