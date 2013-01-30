#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include "compiler/sg/table.h"
#include "declaration_visitor.h"
#include <boost/format.hpp>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

void GenerateMembers(SG::SymbolTable& symbolTable, Diagnostics::Report& report) {
	for(auto& mod : symbolTable) {
		DeclarationVisitor v(*mod, report);

		for(auto& mem : *mod) {
			mem.second->Accept(v);
		}
	}
}

}
}
}
}
