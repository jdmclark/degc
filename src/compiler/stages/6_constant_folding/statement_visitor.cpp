#include "statement_visitor.h"
#include "expression_visitor.h"
#include <boost/format.hpp>
#include <iostream>

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::ConstantFolding::StatementVisitor;

StatementVisitor::StatementVisitor(Deg::Compiler::Diagnostics::Report& report)
	: SG::Visitor("ConstantFolding::StatementVisitor", report) {
	return;
}

void StatementVisitor::VisitCompoundStatement(SG::CompoundStatement& n) {
	for(auto& stmt : n.Statements) {
		stmt->Accept(*this);
	}
}

void StatementVisitor::VisitDisjunctionStatement(SG::DisjunctionStatement& n) {
	for(auto& stmt : n.Statements) {
		stmt->Accept(*this);
	}
}

void StatementVisitor::VisitForAnyStatement(SG::ForAnyStatement& n) {
	n.Code->Accept(*this);
}

void StatementVisitor::VisitForBestStatement(SG::ForBestStatement& n) {
	n.Code->Accept(*this);
}

void StatementVisitor::VisitIfStatement(SG::IfStatement& n) {
	n.Code->Accept(*this);
}

void StatementVisitor::VisitIfElseStatement(SG::IfElseStatement& n) {
	n.Code->Accept(*this);
	n.ElseCode->Accept(*this);
}

void StatementVisitor::VisitTakeStatement(SG::TakeStatement& n) {
	std::vector<std::unique_ptr<SG::Expression>> empty_args;

	try {
		ExpressionVisitor amt_v(empty_args, {}, Report);
		n.Amount->Accept(amt_v);
		n.Amount = std::move(amt_v.GeneratedExpression);
	}
	catch(...) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::TakeAmountNotConstant, Diagnostics::ErrorLevel::Error,
				VisitorName, "take quantity not a constant expression", n.ast_node->Amount->Location));
	}

	try {
		ExpressionVisitor set_v(empty_args, {}, Report);
		n.Set->Accept(set_v);
		n.Set = std::move(set_v.GeneratedExpression);
	}
	catch(...) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::TakeSetNotConstant, Diagnostics::ErrorLevel::Error,
				VisitorName, "take set not a constant expression", n.ast_node->Set->Location));
	}
}

void StatementVisitor::VisitLimitStatement(SG::LimitStatement& n) {
	std::vector<std::unique_ptr<SG::Expression>> empty_args;

	try {
			ExpressionVisitor amt_v(empty_args, {}, Report);
			n.Amount->Accept(amt_v);
			n.Amount = std::move(amt_v.GeneratedExpression);
		}
		catch(...) {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::LimitAmountNotConstant, Diagnostics::ErrorLevel::Error,
					VisitorName, "limit quantity not a constant expression", n.ast_node->Amount->Location));
		}

		try {
			ExpressionVisitor set_v(empty_args, {}, Report);
			n.Set->Accept(set_v);
			n.Set = std::move(set_v.GeneratedExpression);
		}
		catch(...) {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::LimitSetNotConstant, Diagnostics::ErrorLevel::Error,
					VisitorName, "limit set not a constant expression", n.ast_node->Set->Location));
		}
}
