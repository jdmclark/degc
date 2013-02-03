#include "program_base_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateMembers::ProgramBaseVisitor;

ProgramBaseVisitor::ProgramBaseVisitor(SG::ProgramSymbol& program, Diagnostics::Report& report)
	: SG::Visitor("GenerateMembers::ProgramBaseVisitor", report), program(program) {
	return;
}

void ProgramBaseVisitor::DefaultAction(const std::string& action, SG::Node& node) {
	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ProgramExpected, Diagnostics::ErrorLevel::Error,
			VisitorName, "program base is not a program", program.ast_program->Location));
	program.Base = nullptr;
}

void ProgramBaseVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	program.Base = &n;
}
