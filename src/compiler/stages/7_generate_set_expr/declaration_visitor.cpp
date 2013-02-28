#include "declaration_visitor.h"
#include "statement_visitor.h"
#include "expression_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateSetExpressions::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(Diagnostics::Report& report)
	: SG::Visitor("GenerateSetExpressions::DeclarationVisitor", report) {
	return;
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	ExpressionVisitor v(Report);
	n.Code->Accept(v);
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	StatementVisitor v(Report);
	n.Statements->Accept(v);
}
