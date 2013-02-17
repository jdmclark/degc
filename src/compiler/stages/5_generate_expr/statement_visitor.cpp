#include "statement_visitor.h"
#include "expression_visitor.h"
#include "compiler/sg/error_helper.h"
#include <boost/format.hpp>

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateExpressions::StatementVisitor;

StatementVisitor::StatementVisitor(SG::ScopeStack& scope, bool InsideLoop, Diagnostics::Report& report)
	: AST::Visitor("GenerateExpressions::StatementVisitor", report), scope(scope), InsideLoop(InsideLoop) {
	return;
}

void StatementVisitor::VisitNamedStatement(AST::NamedStatement& n) {
	GeneratedStatementName = n.Name;
	n.Code->Accept(*this);
}

void StatementVisitor::VisitCompoundStatement(AST::CompoundStatement& n) {
	std::unique_ptr<SG::CompoundStatement> stmt(new SG::CompoundStatement());

	for(auto& s : *n.Statements) {
		StatementVisitor s_v(scope, InsideLoop, Report);
		s->Accept(s_v);

		std::string stmt_name = s_v.GeneratedStatementName;

		if(!stmt_name.empty() && stmt->Statements.IsMember(stmt_name)) {
			stmt_name.clear();
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::StatementNameRedefinition, Diagnostics::ErrorLevel::Error,
					VisitorName, boost::str(boost::format("statement name \'%s\' is already used") % stmt_name), n.Location));
		}

		if(stmt_name.empty()) {
			stmt_name = boost::str(boost::format("@anon%d") % (stmt->Statements.children_size() + 1));
		}

		stmt->Statements.AddMember(stmt_name, s_v.GeneratedStatement);
	}

	GeneratedStatement = std::move(stmt);
}

void StatementVisitor::VisitAssertStatement(AST::AssertStatement& n) {
	ExpressionVisitor v(scope, Report);
	n.Value->Accept(v);

	SG::BooleanType bt;
	if(bt.CanAcceptValueOfType(*v.GeneratedExpressionType)) {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::AssertStatement(v.GeneratedExpression));
	}
	else {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
		SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.Value->Location);
	}
}

void StatementVisitor::VisitEmbedStatement(AST::EmbedStatement& n) {
	if(InsideLoop) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::EmbedInsideLoop, Diagnostics::ErrorLevel::Error,
				VisitorName, "embedded program inside loop", n.Location));
	}

	ExpressionVisitor v(scope, Report);
	n.Value->Accept(v);

	SG::ProgramType* pt = dynamic_cast<SG::ProgramType*>(v.GeneratedExpressionType.get());
	if(pt) {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::EmbedStatement(v.GeneratedExpression));
	}
	else {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ProgramExpected, Diagnostics::ErrorLevel::Error,
				VisitorName, "embedded value is not a program", n.Value->Location));
	}
}

void StatementVisitor::VisitEmbedInlineStatement(AST::EmbedInlineStatement& n) {
	StatementVisitor v(scope, InsideLoop, Report);
	n.Code->Accept(v);
	GeneratedStatement = std::move(v.GeneratedStatement);
}

void StatementVisitor::VisitDisjunctionStatement(AST::DisjunctionStatement& n) {
	std::unique_ptr<SG::DisjunctionStatement> stmt(new SG::DisjunctionStatement());

	for(auto& s : *n.Statements) {
		StatementVisitor stmt_v(scope, InsideLoop, Report);
		s->Accept(stmt_v);
		stmt->Statements.push_back(std::move(stmt_v.GeneratedStatement));
	}

	GeneratedStatement = std::move(stmt);
}

void StatementVisitor::VisitForAllStatement(AST::ForAllStatement& n) {
	try {
		std::unique_ptr<SG::ForAllStatement> stmt(new SG::ForAllStatement());
		SG::Node& rec_node = scope.GetSymbol(n.Typename);
		SG::RecordSymbol& rec_sym = dynamic_cast<SG::RecordSymbol&>(rec_node);

		ExpressionVisitor set_v(scope, Report);
		n.Set->Accept(set_v);

		SG::SetType* st = dynamic_cast<SG::SetType*>(set_v.GeneratedExpressionType.get());
		if(!st) {
			SG::ErrorHelper::SetExpected(Report, VisitorName, n.Set->Location);
			stmt->Set = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
		}
		else if(st->ElementType != &rec_sym) {
			SG::ErrorHelper::SetDomainMismatch(Report, VisitorName, n.Set->Location);
			stmt->Set = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
		}
		else {
			stmt->Set = std::move(set_v.GeneratedExpression);
		}

		stmt->Iterators.MakeMember<SG::VariableSymbol>(n.ElementName, &rec_sym);

		scope.PushScope(stmt->Iterators);
		StatementVisitor code_v(scope, true, Report);
		n.Code->Accept(code_v);
		scope.PopScope();

		stmt->Code = std::move(code_v.GeneratedStatement);
		GeneratedStatement = std::move(stmt);
	}
	catch(...) {
		SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
	}
}

void StatementVisitor::VisitForAnyStatement(AST::ForAnyStatement& n) {
	try {
		std::unique_ptr<SG::ForAnyStatement> stmt(new SG::ForAnyStatement());
		SG::Node& rec_node = scope.GetSymbol(n.Typename);
		SG::RecordSymbol& rec_sym = dynamic_cast<SG::RecordSymbol&>(rec_node);

		ExpressionVisitor set_v(scope, Report);
		n.Set->Accept(set_v);

		SG::SetType* st = dynamic_cast<SG::SetType*>(set_v.GeneratedExpressionType.get());
		if(!st) {
			SG::ErrorHelper::SetExpected(Report, VisitorName, n.Set->Location);
			stmt->Set = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
		}
		else if(st->ElementType != &rec_sym) {
			SG::ErrorHelper::SetDomainMismatch(Report, VisitorName, n.Set->Location);
			stmt->Set = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
		}
		else {
			stmt->Set = std::move(set_v.GeneratedExpression);
		}

		stmt->Iterators.MakeMember<SG::VariableSymbol>(n.ElementName, &rec_sym);

		scope.PushScope(stmt->Iterators);
		StatementVisitor code_v(scope, InsideLoop, Report);
		n.Code->Accept(code_v);
		scope.PopScope();

		stmt->Code = std::move(code_v.GeneratedStatement);
		GeneratedStatement = std::move(stmt);
	}
	catch(...) {
		SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
	}
}

void StatementVisitor::VisitForBestStatement(AST::ForBestStatement& n) {
	try {
		std::unique_ptr<SG::ForBestStatement> stmt(new SG::ForBestStatement());
		SG::Node& rec_node = scope.GetSymbol(n.Typename);
		SG::RecordSymbol& rec_sym = dynamic_cast<SG::RecordSymbol&>(rec_node);

		ExpressionVisitor set_v(scope, Report);
		n.Set->Accept(set_v);

		SG::SetType* st = dynamic_cast<SG::SetType*>(set_v.GeneratedExpressionType.get());
		if(!st) {
			SG::ErrorHelper::SetExpected(Report, VisitorName, n.Set->Location);
			stmt->Set = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
		}
		else if(st->ElementType != &rec_sym) {
			SG::ErrorHelper::SetDomainMismatch(Report, VisitorName, n.Set->Location);
			stmt->Set = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
		}
		else {
			stmt->Set = std::move(set_v.GeneratedExpression);
		}

		ExpressionVisitor pred_v(scope, Report);
		n.Predicate->Accept(pred_v);

		SG::FunctionType ft;
		ft.ReturnType = std::unique_ptr<SG::Type>(new SG::BooleanType());
		ft.ArgumentTypes.push_back(std::unique_ptr<SG::Type>(new SG::RecordType(&rec_sym)));
		ft.ArgumentTypes.push_back(std::unique_ptr<SG::Type>(new SG::RecordType(&rec_sym)));
		if(!ft.CanAcceptValueOfType(*pred_v.GeneratedExpressionType)) {
			SG::ErrorHelper::PredicateSignatureMismatch(Report, VisitorName, n.Predicate->Location,
					boost::str(boost::format("(%s, %s)->bool") % n.Typename % n.Typename));
			stmt->Predicate = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
		}
		else {
			stmt->Predicate = std::move(pred_v.GeneratedExpression);
		}

		stmt->Iterators.MakeMember<SG::VariableSymbol>(n.ElementName, &rec_sym);

		scope.PushScope(stmt->Iterators);
		StatementVisitor code_v(scope, InsideLoop, Report);
		n.Code->Accept(code_v);
		scope.PopScope();

		stmt->Code = std::move(code_v.GeneratedStatement);
		GeneratedStatement = std::move(stmt);
	}
	catch(...) {
		SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
	}
}

void StatementVisitor::VisitIfStatement(AST::IfStatement& n) {
	ExpressionVisitor pred_v(scope, Report);
	n.Predicate->Accept(pred_v);

	StatementVisitor ifcode_v(scope, InsideLoop, Report);
	n.Code->Accept(ifcode_v);

	SG::BooleanType bt;
	if(bt.CanAcceptValueOfType(*pred_v.GeneratedExpressionType)) {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::IfStatement(pred_v.GeneratedExpression, ifcode_v.GeneratedStatement));
	}
	else {
		SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.Predicate->Location);
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
	}
}

void StatementVisitor::VisitIfElseStatement(AST::IfElseStatement& n) {
	ExpressionVisitor pred_v(scope, Report);
	n.Predicate->Accept(pred_v);

	StatementVisitor ifcode_v(scope, InsideLoop, Report);
	n.Code->Accept(ifcode_v);

	StatementVisitor elsecode_v(scope, InsideLoop, Report);
	n.ElseCode->Accept(elsecode_v);

	SG::BooleanType bt;
	if(bt.CanAcceptValueOfType(*pred_v.GeneratedExpressionType)) {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::IfElseStatement(pred_v.GeneratedExpression, ifcode_v.GeneratedStatement, elsecode_v.GeneratedStatement));
	}
	else {
		SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.Predicate->Location);
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
	}
}

void StatementVisitor::VisitTakeStatement(AST::TakeStatement& n) {
	if(InsideLoop) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::LimitInsideLoop, Diagnostics::ErrorLevel::Error,
				VisitorName, "erroneous take inside loop", n.Location));
	}

	bool error_state = false;

	ExpressionVisitor amt_v(scope, Report);
	n.Amount->Accept(amt_v);

	ExpressionVisitor set_v(scope, Report);
	n.Set->Accept(set_v);

	SG::NumberType bt;
	if(!bt.CanAcceptValueOfType(*amt_v.GeneratedExpressionType)) {
		SG::ErrorHelper::NumberExpected(Report, VisitorName, n.Amount->Location);
		error_state = true;
	}

	SG::SetType* rt = dynamic_cast<SG::SetType*>(set_v.GeneratedExpressionType.get());
	if(!rt) {
		SG::ErrorHelper::SetExpected(Report, VisitorName, n.Set->Location);
		error_state = true;
	}
	else if(rt->ElementType->QuantityMember == nullptr) {
		SG::ErrorHelper::SetDoesNotHaveQuantity(Report, VisitorName, n.Set->Location);
		error_state = true;
	}

	if(error_state) {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
	}
	else {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::TakeStatement(&n, amt_v.GeneratedExpression, set_v.GeneratedExpression));
	}
}

void StatementVisitor::VisitLimitStatement(AST::LimitStatement& n) {
	if(InsideLoop) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::LimitInsideLoop, Diagnostics::ErrorLevel::Error,
				VisitorName, "erroneous limit inside loop", n.Location));
	}

	bool error_state = false;

	ExpressionVisitor amt_v(scope, Report);
	n.Amount->Accept(amt_v);

	ExpressionVisitor set_v(scope, Report);
	n.Set->Accept(set_v);

	SG::NumberType bt;
	if(!bt.CanAcceptValueOfType(*amt_v.GeneratedExpressionType)) {
		SG::ErrorHelper::NumberExpected(Report, VisitorName, n.Amount->Location);
		error_state = true;
	}

	SG::SetType* rt = dynamic_cast<SG::SetType*>(set_v.GeneratedExpressionType.get());
	if(!rt) {
		SG::ErrorHelper::SetExpected(Report, VisitorName, n.Set->Location);
		error_state = true;
	}
	else if(rt->ElementType->QuantityMember == nullptr) {
		SG::ErrorHelper::SetDoesNotHaveQuantity(Report, VisitorName, n.Set->Location);
		error_state = true;
	}

	if(error_state) {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::ErrorStatement());
	}
	else {
		GeneratedStatement = std::unique_ptr<SG::Statement>(new SG::LimitStatement(&n, amt_v.GeneratedExpression, set_v.GeneratedExpression));
	}
}
