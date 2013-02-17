#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include "compiler/sg/table.h"
#include "compiler/sg/scope_stack.h"
#include "declaration_visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace ConstantFolding {

void ConstantFolding(SG::SymbolTable& symbolTable, Diagnostics::Report& report) {
	DeclarationVisitor v(report);
	for(auto& mod : symbolTable) {
		for(auto& mem : *mod) {
			mem.second->Accept(v);
		}
	}
}

}
}
}
}
