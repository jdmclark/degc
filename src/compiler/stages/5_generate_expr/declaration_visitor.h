#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/module.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateExpressions {

class DeclarationVisitor : public SG::Visitor {
private:
	SG::Module& module;

public:
	DeclarationVisitor(SG::Module& module, Diagnostics::Report& report);

	SGCONSUME(NumberSymbol);
	SGCONSUME(QuantitySymbol);
	SGCONSUME(BooleanSymbol);
	SGCONSUME(EnumerationSymbol);
	SGCONSUME(RecordSymbol);

	void VisitProgramSymbol(SG::ProgramSymbol& n);
	void VisitFunctionSymbol(SG::FunctionSymbol& n);
};

}
}
}
}
