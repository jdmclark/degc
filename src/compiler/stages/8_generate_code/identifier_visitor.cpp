#include "identifier_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::IdentifierVisitor;

IdentifierVisitor::IdentifierVisitor(IR::Printer& code, const std::vector<int>& programArguments, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::IdentifierVisitor", report), code(code), programArguments(programArguments) {
	return;
}

void IdentifierVisitor::VisitEnumerationMemberSymbol(EnumerationMemberSymbol& n) {
	code.ConstN(Runtime::Math::DefaultFixed(n.Value));
}

void IdentifierVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	code.ConstF(n.UniversalUniqueName);
}

void IdentifierVisitor::VisitFunctionArgumentSymbol(FunctionArgumentSymbol& n) {
	code.LoadS(n.Index);
}

void IdentifierVisitor::VisitProgramArgumentSymbol(ProgramArgumentSymbol& n) {
	code.ConstN(Runtime::Math::DefaultFixed(programArguments[n.Index]));
}
