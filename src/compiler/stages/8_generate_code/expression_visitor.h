#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/node.h"
#include "compiler/ir/printer.h"
#include "runtime/math/fixed.h"
#include "runtime/code/recordtypetable.h"
#include "runtime/code/programtable.h"
#include <memory>
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class ExpressionVisitor : public SG::Visitor {
private:
	IR::Printer& code;
	Runtime::Code::RecordTypeTable& recordTypeTable;
	const std::vector<SG::EnumerationMemberSymbol*>& programArguments;

public:
	ExpressionVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, const std::vector<SG::EnumerationMemberSymbol*>& programArguments, Diagnostics::Report& report);

	void VisitNumericExpression(SG::NumericExpression& e);
	void VisitBooleanExpression(SG::BooleanExpression& e);
	void VisitTypedSetExpression(SG::TypedSetExpression& e);
	void VisitConstrainedSetExpression(SG::ConstrainedSetExpression& e);
	void VisitPanicExpression(SG::PanicExpression& e);
	void VisitIdentifierExpression(SG::IdentifierExpression& e);
	void VisitFunctionCallExpression(SG::FunctionCallExpression& e);
	void VisitMemberAccessExpression(SG::MemberAccessExpression& e);
	void VisitUnaryExpression(SG::UnaryExpression& e);
	void VisitExistsExpression(SG::ExistsExpression& e);
	void VisitInfixExpression(SG::InfixExpression& e);
	void VisitFunctionIfElseExpression(SG::FunctionIfElseExpression& e);
};

}
}
}
}
