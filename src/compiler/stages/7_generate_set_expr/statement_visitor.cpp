#include "statement_visitor.h"
#include "expression_visitor.h"
#include <boost/format.hpp>
#include <iostream>

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateSetExpressions::StatementVisitor;

StatementVisitor::StatementVisitor(Deg::Compiler::Diagnostics::Report& report)
	: SG::Visitor("GenerateSetExpressions::StatementVisitor", report) {
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

void StatementVisitor::VisitAssertStatement(SG::AssertStatement& n) {
	ExpressionVisitor v(Report);
	n.Value->Accept(v);
}

void StatementVisitor::VisitForAllStatement(SG::ForAllStatement& n) {
	ExpressionVisitor v(Report);
	n.Set->Accept(v);

	n.Code->Accept(*this);
}

void StatementVisitor::VisitForAnyStatement(SG::ForAnyStatement& n) {
	ExpressionVisitor v(Report);
	n.Set->Accept(v);

	n.Code->Accept(*this);
}

void StatementVisitor::VisitForBestStatement(SG::ForBestStatement& n) {
	ExpressionVisitor v(Report);
	n.Predicate->Accept(v);
	n.Set->Accept(v);

	n.Code->Accept(*this);
}

void StatementVisitor::VisitIfStatement(SG::IfStatement& n) {
	ExpressionVisitor v(Report);
	n.Predicate->Accept(v);

	n.Code->Accept(*this);
}

void StatementVisitor::VisitIfElseStatement(SG::IfElseStatement& n) {
	ExpressionVisitor v(Report);
	n.Predicate->Accept(v);

	n.Code->Accept(*this);
	n.ElseCode->Accept(*this);
}

void StatementVisitor::VisitTakeStatement(SG::TakeStatement& n) {
	ExpressionVisitor v(Report);
	n.Amount->Accept(v);
	n.Set->Accept(v);
}

void StatementVisitor::VisitLimitStatement(SG::LimitStatement& n) {
	ExpressionVisitor v(Report);
	n.Amount->Accept(v);
	n.Set->Accept(v);
}
