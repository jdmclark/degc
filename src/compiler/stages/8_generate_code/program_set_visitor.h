#pragma once

#include "compiler/sg/visitor.h"
#include "runtime/math/set.h"
#include "runtime/code/recordtypetable.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class ProgramSetVisitor : public SG::Visitor {
private:
	Runtime::Code::RecordTypeTable& recordTypeTable;

public:
	Runtime::Math::Set result;
	size_t result_record_type;

	ProgramSetVisitor(Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report);

	void VisitConstrainedSetExpression(SG::ConstrainedSetExpression& e);
	void VisitTypedSetExpression(SG::TypedSetExpression& e);
};

}
}
}
}
