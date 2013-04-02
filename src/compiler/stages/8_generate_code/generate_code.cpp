#include "compiler/sg/table.h"
#include "runtime/code/codebuffer.h"
#include "runtime/code/functiontable.h"
#include "compiler/diagnostics/report.h"
#include "declaration_visitor.h"
#include "compiler/ir/printer.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

void GenerateCode(SG::SymbolTable& symbolTable, IR::Printer& code, Diagnostics::Report& report) {
	DeclarationVisitor v(code, report);
	for(auto& mod : symbolTable) {
		for(auto& symbol : *mod) {
			symbol.second->Accept(v);
		}
	}

	code.Backpatch();
	return;
}

}
}
}
}
