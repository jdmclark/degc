#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/scope_stack.h"
#include "compiler/ir/printer.h"
#include "runtime/code/recordtypetable.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class DeclarationVisitor : public SG::Visitor {
private:
	IR::Printer& code;
	Runtime::Code::RecordTypeTable& recordTypeTable;
	
public:
	DeclarationVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report);

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
