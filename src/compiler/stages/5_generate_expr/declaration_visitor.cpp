#include "declaration_visitor.h"
#include "expression_visitor.h"
#include "statement_visitor.h"
#include <boost/format.hpp>

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateExpressions::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(ScopeStack& scope, Diagnostics::Report& report)
	: SG::Visitor("GenerateExpressions::DeclarationVisitor", report), scope(scope) {
	return;
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	scope.PushScope(n.Arguments);

	if(n.Base) {
		if(n.Base->Arguments.children_size() != n.ast_program->BaseArguments->size()) {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ProgramArgCountMismatch, Diagnostics::ErrorLevel::Error,
					VisitorName, boost::str(boost::format("base program expects %d arguments, was passed %d") % n.Base->Arguments.children_size() % n.ast_program->BaseArguments->size()),
					n.ast_program->Location));
		}
		else {
			// Create table of expected types
			std::vector<std::unique_ptr<SG::Type>> expected_types;
			for(auto& arg : n.Base->Arguments) {
				SG::ProgramArgumentSymbol& parg = dynamic_cast<SG::ProgramArgumentSymbol&>(*arg);
				expected_types.push_back(std::move(parg.InputType->Clone()));
			}

			for(size_t i = 0; i < n.Base->Arguments.children_size(); ++i) {
				ExpressionVisitor av(scope, Report);
				(*n.ast_program->BaseArguments)[i]->Accept(av);

				if(expected_types[i]->CanAcceptValueOfType(*av.GeneratedExpressionType)) {
					n.BaseArguments.push_back(std::move(av.GeneratedExpression));
				}
				else {
					Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ProgramArgTypeMismatch, Diagnostics::ErrorLevel::Error,
							VisitorName, boost::str(boost::format("argument %d type mismatch") % (i + 1)), (*n.ast_program->BaseArguments)[i]->Location));
					n.BaseArguments.push_back(std::unique_ptr<SG::Expression>(new SG::ErrorExpression()));
				}
			}
		}
	}

	for(auto arg : *n.ast_program->BaseArguments) {
		ExpressionVisitor ev(scope, Report);
		arg->Accept(ev);
		n.BaseArguments.push_back(std::move(ev.GeneratedExpression));
	}

	StatementVisitor v(scope, false, Report);
	n.ast_program->Code->Accept(v);

	n.Statements = std::move(v.GeneratedStatement);

	scope.PopScope();
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
