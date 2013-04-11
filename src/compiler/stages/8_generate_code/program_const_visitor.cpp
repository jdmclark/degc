#include "program_const_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::ProgramConstVisitor;

ProgramConstVisitor::ProgramConstVisitor(const std::vector<int>& programArguments, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::ProgramConstVisitor", report), programArguments(programArguments) {
	return;
}

void ProgramConstVisitor::VisitNumericExpression(NumericExpression& n) {
	value = n.Value;
}

void ProgramConstVisitor::VisitIdentifierExpression(IdentifierExpression& n) {
	n.ReferencedNode->Accept(*this);
}

void ProgramConstVisitor::VisitEnumerationMemberSymbol(EnumerationMemberSymbol& n) {
	value = Runtime::Math::DefaultFixed(n.Value);
}

void ProgramConstVisitor::VisitProgramArgumentSymbol(ProgramArgumentSymbol& n) {
	value = Runtime::Math::DefaultFixed(programArguments[n.Index]);
}
