#include "declaration_visitor.h"
#include "expression_visitor.h"
#include "statement_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateExpressions::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(ScopeStack& scope, Diagnostics::Report& report)
	: SG::Visitor("GenerateExpressions::DeclarationVisitor", report), scope(scope) {
	return;
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	StatementVisitor v(scope, false, Report);
	n.ast_program->Code->Accept(v);

	n.Statements = std::move(v.GeneratedStatement);
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	scope.PushScope(n.Arguments);

	ExpressionVisitor v(scope, Report);
	n.ast_function->Code->Accept(v);

	n.Code = std::move(v.GeneratedExpression);
	if(!n.CodomainType->CanAcceptValueOfType(*v.GeneratedExpressionType)) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::CodomainTypeMismatch,
				Diagnostics::ErrorLevel::Error, VisitorName, "function value does not match codomain",
				n.ast_function->Code->Location));
	}

	scope.PopScope();
}
