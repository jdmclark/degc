#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include "compiler/sg/table.h"
#include "compiler/sg/scope_stack.h"
#include "declaration_visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateExpressions {

void GenerateExpressions(SG::SymbolTable& symbolTable, Diagnostics::Report& report) {
	for(auto& mod : symbolTable) {
		SG::ScopeStack scope(*mod);
		DeclarationVisitor v(scope, report);

		for(auto& mem : *mod) {
			mem.second->Accept(v);
		}
	}
}

}
}
}
}
