#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/node.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateSetExpressions {

class SetRecordVisitor : public SG::Visitor {
private:
	Diagnostics::ErrorLocation location;
	void DefaultAction(const std::string& action, SG::Node& n);

public:
	SetRecordVisitor(const Diagnostics::ErrorLocation& location, Diagnostics::Report& report);

	bool IsIndependent;
	bool IsRecordMemberSymbol;
	SG::RecordMemberSymbol* Member;

	void VisitNumericExpression(SG::NumericExpression& n);
	void VisitBooleanExpression(SG::BooleanExpression& n);
	void VisitErrorExpression(SG::ErrorExpression& n);
	void VisitIdentifierExpression(SG::IdentifierExpression& n);
	void VisitUnaryExpression(SG::UnaryExpression& n);
	void VisitInfixExpression(SG::InfixExpression& n);
};

}
}
}
}
