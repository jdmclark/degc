#include "set_expression_visitor.h"
#include "expression_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::SetExpressionVisitor;

SetExpressionVisitor::SetExpressionVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, size_t record_width, const std::vector<int>& programArguments, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::SetExpressionVisitor", report), code(code), recordTypeTable(recordTypeTable), record_width(record_width), programArguments(programArguments) {
	return;
}

void SetExpressionVisitor::VisitInfixExpression(InfixExpression& e) {
	e.LeftValue->Accept(*this);
	e.RightValue->Accept(*this);

	switch(e.Operator) {
	case AST::InfixOperator::And:
		code.Intersect();
		return;

	case AST::InfixOperator::Or:
		code.Union();
		return;

	default:
		break;
	}

	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
			VisitorName, "infix operator not implemented"));
}

void SetExpressionVisitor::VisitSetClauseExpression(SetClauseExpression& e) {
	ExpressionVisitor ev(code, recordTypeTable, programArguments, Report);
	e.Value->Accept(ev);

	switch(e.Operator) {
	case AST::InfixOperator::Greater:
		code.ConsGt(record_width, e.Member->Index);
		return;

	case AST::InfixOperator::GreaterEqual:
		code.ConsGeq(record_width, e.Member->Index);
		return;

	case AST::InfixOperator::Less:
		code.ConsLt(record_width, e.Member->Index);
		return;

	case AST::InfixOperator::LessEqual:
		code.ConsLeq(record_width, e.Member->Index);
		return;

	case AST::InfixOperator::Equal:
		code.ConsEq(record_width, e.Member->Index);
		return;

	case AST::InfixOperator::NotEqual:
		code.ConsNeq(record_width, e.Member->Index);
		return;

	default:
		break;
	}

	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
			VisitorName, "set clause operator not implemented"));
}
