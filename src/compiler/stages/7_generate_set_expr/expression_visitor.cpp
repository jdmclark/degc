#include "expression_visitor.h"
#include "set_filter_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateSetExpressions::ExpressionVisitor;

ExpressionVisitor::ExpressionVisitor(Diagnostics::Report& report) : SG::Visitor("GenerateSetExpressions::ExpressionVisitor", report) {
	return;
}

void ExpressionVisitor::VisitConstrainedSetExpression(SG::ConstrainedSetExpression& n) {
	SetFilterVisitor v(n.Location, Report);
	n.Filter->Accept(v);

	n.Filter = std::move(v.GeneratedExpression);
}

void ExpressionVisitor::VisitFunctionCallExpression(SG::FunctionCallExpression& n) {
	n.FunctionTargetExpression->Accept(*this);

	for(auto& arg : n.ArgumentExpressions) {
		arg->Accept(*this);
	}
}

void ExpressionVisitor::VisitUnaryExpression(SG::UnaryExpression& n) {
	n.Value->Accept(*this);
}

void ExpressionVisitor::VisitExistsExpression(SG::ExistsExpression& n) {
	n.Value->Accept(*this);
}

void ExpressionVisitor::VisitInfixExpression(SG::InfixExpression& n) {
	n.LeftValue->Accept(*this);
	n.RightValue->Accept(*this);
}

void ExpressionVisitor::VisitFunctionIfElseExpression(SG::FunctionIfElseExpression& n) {
	n.Predicate->Accept(*this);
	n.IfCode->Accept(*this);
	n.ElseCode->Accept(*this);
}
