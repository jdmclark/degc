#include "declaration_visitor.h"
#include "expression_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(IR::Printer& code, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::DeclarationVisitor", report), code(code) {
	return;
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	code.Function(n.UniversalUniqueName);
	ExpressionVisitor v(code, Report);
	n.Code->Accept(v);
	code.Ret();
}
