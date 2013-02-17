#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/scope_stack.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace ConstantFolding {

class DeclarationVisitor : public SG::Visitor {
public:
	DeclarationVisitor(Diagnostics::Report& report);

	SGCONSUME(NumberSymbol);
	SGCONSUME(QuantitySymbol);
	SGCONSUME(BooleanSymbol);
	SGCONSUME(EnumerationSymbol);
	SGCONSUME(RecordSymbol);
	SGCONSUME(FunctionSymbol);

	void VisitProgramSymbol(SG::ProgramSymbol& n);
};

}
}
}
}
