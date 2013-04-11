#pragma once

#include "runtime/math/set.h"
#include "compiler/sg/visitor.h"
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class ProgramSetExpressionVisitor : public SG::Visitor {
private:
	size_t record_width;
	const std::vector<SG::EnumerationMemberSymbol*>& programArguments;

public:
	Runtime::Math::Set result;

	ProgramSetExpressionVisitor(size_t record_width, const std::vector<SG::EnumerationMemberSymbol*>& programArguments, Diagnostics::Report& report);

	void VisitInfixExpression(SG::InfixExpression& e);
	void VisitSetClauseExpression(SG::SetClauseExpression& e);
};

}
}
}
}
