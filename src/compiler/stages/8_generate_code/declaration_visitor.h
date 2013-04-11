#pragma once

#include "compiler/sg/visitor.h"
#include "compiler/sg/scope_stack.h"
#include "compiler/ir/printer.h"
#include "runtime/code/recordtypetable.h"
#include "runtime/code/programtable.h"
#include "runtime/solver/program.h"
#include "program_visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class DeclarationVisitor : public SG::Visitor {
private:
	IR::Printer& code;
	Runtime::Code::RecordTypeTable& recordTypeTable;
	Runtime::Code::ProgramTable& programTable;
	
	std::unique_ptr<Runtime::Solver::ProgramNetworkReified> ReifyProgram(SG::ProgramSymbol& n, const std::vector<int>& params, const std::vector<SG::EnumerationSymbol*>& value_types);

public:
	DeclarationVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Runtime::Code::ProgramTable& programTable, Diagnostics::Report& report);

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
