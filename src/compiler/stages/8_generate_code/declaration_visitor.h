#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/scope_stack.h"
#include "compiler/ir/printer.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class DeclarationVisitor : public SG::Visitor {
private:
	IR::Printer& code;
	
public:
	DeclarationVisitor(IR::Printer& code, Diagnostics::Report& report);

	SGCONSUME(NumberSymbol);
	SGCONSUME(QuantitySymbol);
	SGCONSUME(BooleanSymbol);
	SGCONSUME(EnumerationSymbol);
	SGCONSUME(RecordSymbol);
	SGCONSUME(ProgramSymbol);

	void VisitFunctionSymbol(SG::FunctionSymbol& n);
};

}
}
}
}
