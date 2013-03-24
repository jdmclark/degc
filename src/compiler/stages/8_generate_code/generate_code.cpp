#include "compiler/sg/table.h"
#include "runtime/code/codebuffer.h"
#include "runtime/code/functiontable.h"
#include "compiler/diagnostics/report.h"
#include "declaration_visitor.h"
#include "compiler/ir/codeprinter.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

void GenerateCode(SG::SymbolTable& symbolTable, Runtime::Code::CodeBuffer& codeBuffer, Runtime::Code::FunctionTable& functionTable, Diagnostics::Report& report) {
	IR::CodePrinter cp(codeBuffer, functionTable);
	DeclarationVisitor v(cp, report);
	for(auto& mod : symbolTable) {
		for(auto& symbol : *mod) {
			symbol.second->Accept(v);
		}
	}

	cp.Backpatch();
	return;
}

}
}
}
}