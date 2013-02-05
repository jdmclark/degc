#pragma once

#include "compiler/sg/scope_stack.h"
#include "compiler/ast/visitor.h"
#include <memory>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateExpressions {

class ExpressionVisitor : public AST::Visitor {
private:
	SG::ScopeStack& scope;

public:
	ExpressionVisitor(SG::ScopeStack& scope, Diagnostics::Report& report);

	bool IsConstantValue;
	std::unique_ptr<SG::Expression> GeneratedExpression;
	std::unique_ptr<SG::Type> GeneratedExpressionType;

	void VisitNumericLiteralExpression(AST::NumericLiteralExpression& n);
	void VisitBooleanLiteralExpression(AST::BooleanLiteralExpression& n);
	void VisitTypedSetExpression(AST::TypedSetExpression& n);
	/*void VisitConstrainedSetExpression(AST::ConstrainedSetExpression& n);
	void VisitPanicExpression(AST::PanicExpression& n);
	void VisitIdentifierExpression(AST::IdentifierExpression& n);
	void VisitFunctionCallExpression(AST::FunctionCallExpression& n);
	void VisitMemberAccessExpression(AST::MemberAccessExpression& n);
	void VisitUnaryExpression(AST::UnaryExpression& n);
	void VisitInfixExpression(AST::InfixExpression& n);
	void VisitFunctionIfElseExpression(AST::FunctionIfElseExpression& n);*/
};

}
}
}
}
