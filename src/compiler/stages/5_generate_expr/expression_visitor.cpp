#include "expression_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateExpressions::ExpressionVisitor;

ExpressionVisitor::ExpressionVisitor(SG::ScopeStack& scope, Diagnostics::Report& report)
	: AST::Visitor("GenerateExpressions::ExpressionVisitor", report), scope(scope), IsConstantValue(false) {
	return;
}

void ExpressionVisitor::VisitNumericLiteralExpression(AST::NumericLiteralExpression& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(n.Value));
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::NumberType());
}

void ExpressionVisitor::VisitBooleanLiteralExpression(AST::BooleanLiteralExpression& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(n.Value));
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
}

void ExpressionVisitor::VisitEmptySetExpression(AST::EmptySetExpression& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::EmptySetExpression());
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::SetType());
}

void ExpressionVisitor::VisitUniversalSetExpression(AST::UniversalSetExpression& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::UniversalSetExpression());
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::SetType());
}

void ExpressionVisitor::VisitTypedSetExpression(AST::TypedSetExpression& n) {
	IsConstantValue = true;

	try {
		SG::Node& element_node = scope.GetSymbol(n.Typename);
		SG::RecordSymbol* rec_node = dynamic_cast<SG::RecordSymbol*>(&element_node);
		if(rec_node) {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::TypedSetExpression(rec_node));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::ConstrainedSetType(rec_node));
			return;
		}
	}
	catch(...) {
		// Consume exception
	}

	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::ErrorType());
	SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
}

/*void ExpressionVisitor::VisitConstrainedSetExpression(AST::ConstrainedSetExpression& n) {

}

void ExpressionVisitor::VisitPanicExpression(AST::PanicExpression& n) {

}

void ExpressionVisitor::VisitIdentifierExpression(AST::IdentifierExpression& n) {

}

void ExpressionVisitor::VisitFunctionCallExpression(AST::FunctionCallExpression& n) {

}

void ExpressionVisitor::VisitMemberAccessExpression(AST::MemberAccessExpression& n) {

}

void ExpressionVisitor::VisitUnaryExpression(AST::UnaryExpression& n) {

}

void ExpressionVisitor::VisitInfixExpression(AST::InfixExpression& n) {

}*/
