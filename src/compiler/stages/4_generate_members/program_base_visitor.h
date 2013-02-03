#pragma once

#include "compiler/sg/visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class ProgramBaseVisitor : public SG::Visitor {
private:
	SG::ProgramSymbol& program;

public:
	ProgramBaseVisitor(SG::ProgramSymbol& program, Diagnostics::Report& report);

	void DefaultAction(const std::string& action, SG::Node& node);

	void VisitProgramSymbol(SG::ProgramSymbol& n);
};

}
}
}
}
