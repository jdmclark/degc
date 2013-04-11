#pragma once

#include "compiler/ir/printer.h"
#include "compiler/sg/visitor.h"
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class IdentifierVisitor : public SG::Visitor {
private:
	IR::Printer& code;
	const std::vector<int>& programArguments;
public:
	IdentifierVisitor(IR::Printer& code, const std::vector<int>& programArguments, Diagnostics::Report& report);

	void VisitEnumerationMemberSymbol(SG::EnumerationMemberSymbol& n);
	void VisitFunctionSymbol(SG::FunctionSymbol& n);
	void VisitFunctionArgumentSymbol(SG::FunctionArgumentSymbol& n);
	void VisitProgramArgumentSymbol(SG::ProgramArgumentSymbol& n);
};

}
}
}
}
