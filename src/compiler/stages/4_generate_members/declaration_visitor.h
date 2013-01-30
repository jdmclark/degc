#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/module.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class DeclarationVisitor : public SG::Visitor {
protected:
	SG::Module& module;

public:
	DeclarationVisitor(SG::Module& module, Diagnostics::Report& report);

	SGCONSUME(NumberSymbol);
	SGCONSUME(QuantitySymbol);
	SGCONSUME(BooleanSymbol);
	SGCONSUME(SetSymbol);

	void VisitProgramSymbol(SG::ProgramSymbol& n);
	void VisitRecordSymbol(SG::RecordSymbol& n);
	void VisitFunctionSymbol(SG::FunctionSymbol& n);
	void VisitEnumerationSymbol(SG::EnumerationSymbol& n);
};

}
}
}
}
