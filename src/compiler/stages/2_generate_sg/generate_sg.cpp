#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include "compiler/sg/table.h"
#include "declaration_visitor.h"

void Deg::Compiler::Stages::GenerateSG::GenerateSG(const std::vector<AST::TranslationUnit*>& units,
		SG::SymbolTable& symbolTable, Diagnostics::Report& report) {
	// Generate root module
	SG::Module& root = symbolTable.CreateModule("");
	root.MakeSymbol<SG::NumberSymbol>("number");
	root.MakeSymbol<SG::QuantitySymbol>("quantity");
	root.MakeSymbol<SG::BooleanSymbol>("bool");
	root.MakeSymbol<SG::SetSymbol>("set");

	for(auto unit : units) {
		SG::Module& mod = symbolTable.CreateModule(unit->Name->Name);

		DeclarationVisitor v(mod, report);
		for(auto node : *unit->Declarations) {
			node->Accept(v);
		}
	}
}
