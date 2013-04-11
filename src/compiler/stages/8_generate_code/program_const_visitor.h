#pragma once

#include "compiler/sg/visitor.h"
#include "runtime/math/fixed.h"
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class ProgramConstVisitor : public SG::Visitor {
private:
	const std::vector<SG::EnumerationMemberSymbol*>& programArguments;

public:
	Runtime::Math::DefaultFixed value;

	ProgramConstVisitor(const std::vector<SG::EnumerationMemberSymbol*>& programArguments, Diagnostics::Report& report);

	void VisitNumericExpression(SG::NumericExpression& n);
	void VisitIdentifierExpression(SG::IdentifierExpression& n);
	void VisitEnumerationMemberSymbol(SG::EnumerationMemberSymbol& n);
	void VisitProgramArgumentSymbol(SG::ProgramArgumentSymbol& n);
};

}
}
}
}
