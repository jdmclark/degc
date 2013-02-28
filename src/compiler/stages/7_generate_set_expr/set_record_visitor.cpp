#include "set_record_visitor.h"
#include "set_filter_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateSetExpressions::SetRecordVisitor;

SetRecordVisitor::SetRecordVisitor(const Diagnostics::ErrorLocation& location, Diagnostics::Report& report)
	: SG::Visitor("GenerateSetExpressions::SetRecordVisitor", report), location(location),
	  IsIndependent(false), IsRecordMemberSymbol(false), Member(nullptr) {
	return;
}

void SetRecordVisitor::DefaultAction(const std::string& action, SG::Node& n) {
	IsIndependent = false;
	IsRecordMemberSymbol = false;
	SG::ErrorHelper::SetFilterNotOrthogonal(Report, VisitorName, location);
}

void SetRecordVisitor::VisitNumericExpression(SG::NumericExpression& n) {
	IsIndependent = true;
	IsRecordMemberSymbol = false;
}

void SetRecordVisitor::VisitBooleanExpression(SG::BooleanExpression& n) {
	IsIndependent = true;
	IsRecordMemberSymbol = false;
}

void SetRecordVisitor::VisitErrorExpression(SG::ErrorExpression& n) {
	IsIndependent = true;
	IsRecordMemberSymbol = false;
}

void SetRecordVisitor::VisitIdentifierExpression(SG::IdentifierExpression& n) {
	SG::RecordMemberSymbol* member = dynamic_cast<SG::RecordMemberSymbol*>(n.ReferencedNode);
	if(member) {
		IsIndependent = false;
		IsRecordMemberSymbol = true;
		Member = member;
	}
	else {
		IsIndependent = true;
		IsRecordMemberSymbol = false;
	}
}

void SetRecordVisitor::VisitUnaryExpression(SG::UnaryExpression& n) {
	SetRecordVisitor v(location, Report);
	n.Value->Accept(v);
	IsIndependent = v.IsIndependent;
	IsRecordMemberSymbol = false;
}

void SetRecordVisitor::VisitInfixExpression(SG::InfixExpression& n) {
	SetRecordVisitor left_v(location, Report);
	n.LeftValue->Accept(left_v);

	SetRecordVisitor right_v(location, Report);
	n.RightValue->Accept(right_v);

	IsIndependent = left_v.IsIndependent && right_v.IsIndependent;
	IsRecordMemberSymbol = false;
}
