#pragma once

#include <memory>
#include <string>
#include "compiler/sg/scope_stack.h"
#include "compiler/sg/visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateExpressions {

class IdentifierVisitor : public SG::Visitor {
private:
	const std::string& symbolname;
	const Diagnostics::ErrorLocation& location;

public:
	IdentifierVisitor(const std::string& symbolname, const Diagnostics::ErrorLocation& location, Diagnostics::Report& report);
	void DefaultAction(const std::string& action, SG::Node& n);

	std::unique_ptr<SG::Expression> GeneratedExpression;
	std::unique_ptr<SG::Type> GeneratedExpressionType;

	void VisitProgramSymbol(SG::ProgramSymbol& n);
	void VisitRecordMemberSymbol(SG::RecordMemberSymbol& n);
	void VisitProgramArgumentSymbol(SG::ProgramArgumentSymbol& n);
	void VisitFunctionArgumentSymbol(SG::FunctionArgumentSymbol& n);
	void VisitFunctionSymbol(SG::FunctionSymbol& n);
};

}
}
}
}
