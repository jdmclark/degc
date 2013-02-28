#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/scope_stack.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateSetExpressions {

class DeclarationVisitor : public SG::Visitor {
public:
	DeclarationVisitor(Diagnostics::Report& report);

	SGCONSUME(NumberSymbol);
	SGCONSUME(QuantitySymbol);
	SGCONSUME(BooleanSymbol);
	SGCONSUME(EnumerationSymbol);
	SGCONSUME(RecordSymbol);

	void VisitFunctionSymbol(SG::FunctionSymbol& n);
	void VisitProgramSymbol(SG::ProgramSymbol& n);
};

}
}
}
}
