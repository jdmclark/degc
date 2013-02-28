#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/node.h"
#include "runtime/math/fixed.h"
#include <memory>
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateSetExpressions {

class ExpressionVisitor : public SG::Visitor {
public:
	ExpressionVisitor(Diagnostics::Report& report);

	SGCONSUME(NumericExpression);
	SGCONSUME(BooleanExpression);
	SGCONSUME(TypedSetExpression);
	SGCONSUME(PanicExpression);
	SGCONSUME(ErrorExpression);
	SGCONSUME(IdentifierExpression);

	void VisitConstrainedSetExpression(SG::ConstrainedSetExpression& n);
	void VisitFunctionCallExpression(SG::FunctionCallExpression& n);
	void VisitUnaryExpression(SG::UnaryExpression& n);
	void VisitExistsExpression(SG::ExistsExpression& n);
	void VisitInfixExpression(SG::InfixExpression& n);
	void VisitFunctionIfElseExpression(SG::FunctionIfElseExpression& n);
};

}
}
}
}
