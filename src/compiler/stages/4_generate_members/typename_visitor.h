#pragma once

#include "compiler/sg/visitor.h"
#include <memory>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class TypenameVisitor : public SG::Visitor {
public:
	TypenameVisitor(Diagnostics::Report& report);

	std::unique_ptr<SG::Type> TypenameType;
	bool IsQuantityType;

	void VisitNumberSymbol(SG::NumberSymbol& n);
	void VisitQuantitySymbol(SG::QuantitySymbol& n);
	void VisitBooleanSymbol(SG::BooleanSymbol& n);
	void VisitSetSymbol(SG::SetSymbol& n);
	void VisitRecordSymbol(SG::RecordSymbol& n);
	void VisitEnumerationSymbol(SG::EnumerationSymbol& n);
};

}
}
}
}
