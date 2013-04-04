#pragma once

#include "compiler/sg/visitor.h"
#include "runtime/math/fixed.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class ProgramConstVisitor : public SG::Visitor {
public:
	Runtime::Math::DefaultFixed value;

	ProgramConstVisitor(Diagnostics::Report& report);

	void VisitNumericExpression(SG::NumericExpression& n);
	void VisitIdentifierExpression(SG::IdentifierExpression& n);
	void VisitEnumerationMemberSymbol(SG::EnumerationMemberSymbol& n);
};

}
}
}
}
