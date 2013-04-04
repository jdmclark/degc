#include "program_set_expression_visitor.h"
#include "program_const_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::ProgramSetExpressionVisitor;
using namespace Deg::Runtime::Math;

ProgramSetExpressionVisitor::ProgramSetExpressionVisitor(size_t record_width, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::ProgramSetExpressionVisitor", report), record_width(record_width), result(record_width) {
	return;
}

void ProgramSetExpressionVisitor::VisitInfixExpression(InfixExpression& e) {
	ProgramSetExpressionVisitor left_v(record_width, Report);
	ProgramSetExpressionVisitor right_v(record_width, Report);

	e.LeftValue->Accept(left_v);
	e.RightValue->Accept(right_v);

	switch(e.Operator) {
	case AST::InfixOperator::And:
		result = left_v.result & right_v.result;
		return;

	case AST::InfixOperator::Or:
		result = left_v.result | right_v.result;
		return;

	default:
		break;
	}

	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
			VisitorName, "infix operator not implemented"));
}

void ProgramSetExpressionVisitor::VisitSetClauseExpression(SetClauseExpression& e) {
	ProgramConstVisitor cv(Report);
	e.Value->Accept(cv);

	switch(e.Operator) {
	case AST::InfixOperator::Greater:
		result = Set(record_width, e.Member->Index, Relation::Greater, cv.value);
		return;

	case AST::InfixOperator::GreaterEqual:
		result = Set(record_width, e.Member->Index, Relation::GreaterEqual, cv.value);
		return;

	case AST::InfixOperator::Less:
		result = Set(record_width, e.Member->Index, Relation::Less, cv.value);
		return;

	case AST::InfixOperator::LessEqual:
		result = Set(record_width, e.Member->Index, Relation::LessEqual, cv.value);
		return;

	case AST::InfixOperator::Equal:
		result = Set(record_width, e.Member->Index, Relation::Equal, cv.value);
		return;

	case AST::InfixOperator::NotEqual:
		result = Set(record_width, e.Member->Index, Relation::NotEqual, cv.value);
		return;

	default:
		break;
	}

	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
			VisitorName, "set clause operator not implemented"));
}
