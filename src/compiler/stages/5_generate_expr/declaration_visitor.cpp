#include "declaration_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateExpressions::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(Module& module, Diagnostics::Report& report)
	: SG::Visitor("GenerateExpressions::DeclarationVisitor", report), module(module) {
	return;
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {

}
