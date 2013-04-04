#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include "compiler/sg/table.h"
#include "declaration_visitor.h"
#include "runtime/code/recordtypetable.h"
#include <boost/format.hpp>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

void GenerateMembers(SG::SymbolTable& symbolTable, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report) {
	for(auto& mod : symbolTable) {
		DeclarationVisitor v(*mod, recordTypeTable, report);

		for(auto& mem : *mod) {
			mem.second->Accept(v);
		}
	}
}

}
}
}
}
