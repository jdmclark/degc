#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/node.h"
#include "compiler/ir/printer.h"
#include "runtime/math/fixed.h"
#include "runtime/code/recordtypetable.h"
#include <memory>
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class SetExpressionVisitor : public SG::Visitor {
private:
	IR::Printer& code;
	Runtime::Code::RecordTypeTable& recordTypeTable;
	size_t record_width;

public:
	SetExpressionVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, size_t record_width, Diagnostics::Report& report);

	void VisitInfixExpression(SG::InfixExpression& e);
	void VisitSetClauseExpression(SG::SetClauseExpression& e);
};

}
}
}
}
