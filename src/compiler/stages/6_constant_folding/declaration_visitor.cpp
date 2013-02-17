#include "declaration_visitor.h"
#include "statement_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::ConstantFolding::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(Diagnostics::Report& report)
	: SG::Visitor("ConstantFolding::DeclarationVisitor", report) {
	return;
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	StatementVisitor v(Report);
	n.Statements->Accept(v);
}
