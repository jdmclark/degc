#include "set_filter_visitor.h"
#include "set_record_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateSetExpressions::SetFilterVisitor;

void SetFilterVisitor::GenerateErrorResult() {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
}

void SetFilterVisitor::DefaultAction(const std::string& action, SG::Node& n) {
	ErrorHelper::SetFilterIrrelevant(Report, VisitorName, Location);
	GenerateErrorResult();
}

SetFilterVisitor::SetFilterVisitor(const Diagnostics::ErrorLocation& Location, Diagnostics::Report& report, bool LogicalInverse)
	: SG::Visitor("GenerateSetExpressions::SetFilterVisitor", report), Location(Location), LogicalInverse(LogicalInverse) {
	return;
}

Deg::Compiler::AST::InfixOperator SetFilterVisitor::ReverseOperator(AST::InfixOperator op) {
	switch(op) {
	case AST::InfixOperator::Less:
		return AST::InfixOperator::Greater;

	case AST::InfixOperator::Greater:
		return AST::InfixOperator::Less;

	case AST::InfixOperator::LessEqual:
		return AST::InfixOperator::GreaterEqual;

	case AST::InfixOperator::GreaterEqual:
		return AST::InfixOperator::LessEqual;

	default:
		return op;
	}
}

Deg::Compiler::AST::InfixOperator SetFilterVisitor::NegateOperator(AST::InfixOperator op) {
	switch(op) {
	case AST::InfixOperator::Less:
		return AST::InfixOperator::GreaterEqual;

	case AST::InfixOperator::Greater:
		return AST::InfixOperator::LessEqual;

	case AST::InfixOperator::LessEqual:
		return AST::InfixOperator::Greater;

	case AST::InfixOperator::GreaterEqual:
		return AST::InfixOperator::Less;

	case AST::InfixOperator::Equal:
		return AST::InfixOperator::NotEqual;

	case AST::InfixOperator::NotEqual:
		return AST::InfixOperator::Equal;

	case AST::InfixOperator::And:
		return AST::InfixOperator::Or;

	case AST::InfixOperator::Or:
		return AST::InfixOperator::And;

	default:
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
				VisitorName, "infix operator negation not implemented", Location));
		return op;
	}
}

bool SetFilterVisitor::OperatorIsRelation(AST::InfixOperator op) {
	switch(op) {
	case AST::InfixOperator::Less:
	case AST::InfixOperator::Greater:
	case AST::InfixOperator::LessEqual:
	case AST::InfixOperator::GreaterEqual:
	case AST::InfixOperator::Equal:
	case AST::InfixOperator::NotEqual:
		return true;

	default:
		return false;
	}
}

void SetFilterVisitor::VisitIdentifierExpression(SG::IdentifierExpression& n) {
	SG::RecordMemberSymbol* member = dynamic_cast<SG::RecordMemberSymbol*>(n.ReferencedNode);
	if(member) {
		AST::InfixOperator eq_op = LogicalInverse ? AST::InfixOperator::NotEqual : AST::InfixOperator::Equal;
		std::unique_ptr<SG::Expression> true_exp(new SG::BooleanExpression(true));
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::SetClauseExpression(member, eq_op, true_exp));
	}
	else {
		DefaultAction("IdentifierExpression", n);
	}
}

void SetFilterVisitor::VisitUnaryExpression(SG::UnaryExpression& n) {
	switch(n.Operator) {
	case AST::UnaryOperator::Not: {
		SetFilterVisitor v(Location, Report, !LogicalInverse);
		GeneratedExpression = std::move(v.GeneratedExpression);
	}
	break;

	default:
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
				VisitorName, "unary expression operator not implemented", Location));
		GenerateErrorResult();
		break;
	}
}

void SetFilterVisitor::VisitInfixExpression(SG::InfixExpression& n) {
	if(!OperatorIsRelation(n.Operator)) {
		SetFilterVisitor left_v(Location, Report, LogicalInverse);
		n.LeftValue->Accept(left_v);

		SetFilterVisitor right_v(Location, Report, LogicalInverse);
		n.RightValue->Accept(right_v);

		AST::InfixOperator op = LogicalInverse ? NegateOperator(n.Operator) : n.Operator;

		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, op));
	}
	else {
		SetRecordVisitor left_v(Location, Report);
		n.LeftValue->Accept(left_v);

		SetRecordVisitor right_v(Location, Report);
		n.RightValue->Accept(right_v);

		AST::InfixOperator op = LogicalInverse ? NegateOperator(n.Operator) : n.Operator;

		if(left_v.IsRecordMemberSymbol && right_v.IsIndependent) {
			// x < 5
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::SetClauseExpression(left_v.Member, op, n.RightValue));
		}
		else if(right_v.IsRecordMemberSymbol && left_v.IsIndependent) {
			// 5 < x
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::SetClauseExpression(right_v.Member, ReverseOperator(op), n.LeftValue));
		}
		else if(left_v.IsIndependent && right_v.IsIndependent) {
			SG::ErrorHelper::SetFilterIrrelevant(Report, VisitorName, Location);
			GenerateErrorResult();
		}
		else {
			SG::ErrorHelper::SetFilterNotOrthogonal(Report, VisitorName, Location);
			GenerateErrorResult();
		}
	}
}
