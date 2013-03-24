#include "identifier_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::IdentifierVisitor;

IdentifierVisitor::IdentifierVisitor(IR::Printer& code, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::IdentifierVisitor", report), code(code) {
	return;
}

void IdentifierVisitor::VisitFunctionArgumentSymbol(FunctionArgumentSymbol& n) {
	code.LoadS(n.Index);
}
