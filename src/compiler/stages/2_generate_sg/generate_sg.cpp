#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include "compiler/sg/table.h"
#include "declaration_visitor.h"

void Deg::Compiler::Stages::GenerateSG::GenerateSG(const std::vector<AST::TranslationUnit*>& units,
		SG::SymbolTable& symbolTable, Diagnostics::Report& report) {
	for(auto unit : units) {
		SG::Module& mod = symbolTable.CreateModule(unit->Name->Name);

		DeclarationVisitor v(mod, report);
		for(auto node : *unit->Declarations) {
			node->Accept(v);
		}
	}
}
