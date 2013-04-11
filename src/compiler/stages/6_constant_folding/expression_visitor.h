#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/node.h"
#include "runtime/math/fixed.h"
#include <memory>
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace ConstantFolding {

class ExpressionVisitor : public SG::Visitor {
	bool GetConstantBoolean();
	Runtime::Math::DefaultFixed GetConstantNumber();
	SG::RecordSymbol* GetConstantSetSymbol();
	std::unique_ptr<SG::Expression> GetConstantSetFilter();
	Diagnostics::ErrorLocation GetConstantSetLocation();
	bool GetConstantEquals(SG::Expression& a, SG::Expression& b);

public:
	ExpressionVisitor(const std::vector<std::unique_ptr<SG::Expression>>& FunctionArguments, const std::vector<SG::EnumerationMemberSymbol*>& ProgramArguments, Diagnostics::Report& report);

	const std::vector<std::unique_ptr<SG::Expression>>& FunctionArguments;
	const std::vector<SG::EnumerationMemberSymbol*>& ProgramArguments;
	std::unique_ptr<SG::Expression> GeneratedExpression;
	bool ResultFoldable;

	void DefaultAction(const std::string& action, SG::Node& n);

	void VisitProgramSymbol(SG::ProgramSymbol& n);
	void VisitProgramArgumentSymbol(SG::ProgramArgumentSymbol& n);
	void VisitFunctionArgumentSymbol(SG::FunctionArgumentSymbol& n);
	void VisitFunctionSymbol(SG::FunctionSymbol& n);
	void VisitEnumerationMemberSymbol(SG::EnumerationMemberSymbol& n);
	void VisitRecordMemberSymbol(SG::RecordMemberSymbol& n);

	void VisitNumericExpression(SG::NumericExpression& n);
	void VisitBooleanExpression(SG::BooleanExpression& n);
	void VisitTypedSetExpression(SG::TypedSetExpression& n);
	void VisitConstrainedSetExpression(SG::ConstrainedSetExpression& n);
	void VisitPanicExpression(SG::PanicExpression& n);
	void VisitIdentifierExpression(SG::IdentifierExpression& n);
	void VisitFunctionCallExpression(SG::FunctionCallExpression& n);
	void VisitUnaryExpression(SG::UnaryExpression& n);
	void VisitExistsExpression(SG::ExistsExpression& n);
	void VisitInfixExpression(SG::InfixExpression& n);
	void VisitFunctionIfElseExpression(SG::FunctionIfElseExpression& n);
};

}
}
}
}
