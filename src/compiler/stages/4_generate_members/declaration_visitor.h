#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/module.h"
#include "runtime/code/recordtypetable.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class DeclarationVisitor : public SG::Visitor {
protected:
	SG::Module& module;
	Runtime::Code::RecordTypeTable& recordTypeTable;

public:
	DeclarationVisitor(SG::Module& module, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report);

	SGCONSUME(NumberSymbol);
	SGCONSUME(QuantitySymbol);
	SGCONSUME(BooleanSymbol);

	void VisitProgramSymbol(SG::ProgramSymbol& n);
	void VisitRecordSymbol(SG::RecordSymbol& n);
	void VisitFunctionSymbol(SG::FunctionSymbol& n);
	void VisitEnumerationSymbol(SG::EnumerationSymbol& n);
};

}
}
}
}
