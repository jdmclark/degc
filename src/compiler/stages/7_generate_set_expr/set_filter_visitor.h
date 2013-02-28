#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/node.h"
#include <memory>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateSetExpressions {

class SetFilterVisitor : public SG::Visitor {
private:
	Diagnostics::ErrorLocation Location;

	void GenerateErrorResult();
	void DefaultAction(const std::string& action, SG::Node& n);

	AST::InfixOperator ReverseOperator(AST::InfixOperator op);
	AST::InfixOperator NegateOperator(AST::InfixOperator op);
	bool OperatorIsRelation(AST::InfixOperator op);

	bool LogicalInverse;

public:
	SetFilterVisitor(const Diagnostics::ErrorLocation& Location, Diagnostics::Report& report, bool LogicalInverse = false);

	void VisitIdentifierExpression(SG::IdentifierExpression& n);
	void VisitUnaryExpression(SG::UnaryExpression& n);
	void VisitInfixExpression(SG::InfixExpression& n);

	std::unique_ptr<SG::Expression> GeneratedExpression;
};

}
}
}
}
