#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include "compiler/sg/table.h"
#include "compiler/sg/error_helper.h"
#include <boost/format.hpp>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace ResolveImports {

void AddModuleImport(SG::Module& mod, const std::string& name, SG::Symbol* sym, AST::SymbolImport& isym, Diagnostics::Report& report) {
	try {
		mod.ImportSymbol(name, sym);
	}
	catch(...) {
		SG::ErrorHelper::SymbolRedefinition(report, "ResolveImports", isym.Location, name);
	}
}

void AddModuleImportError(SG::Module& mod, const std::string& name, AST::SymbolImport& isym, Diagnostics::Report& report) {
	try {
		mod.MakeSymbol<SG::ErrorSymbol>(name);
	}
	catch(...) {
		SG::ErrorHelper::SymbolRedefinition(report, "ResolveImports", isym.Location, name);
	}
}

void ResolveImports(const std::vector<AST::TranslationUnit*>& units, SG::SymbolTable& symbolTable, Diagnostics::Report& report) {
	for(auto unit : units) {
		SG::Module& mod = symbolTable.CreateModule(unit->Name->Name);

		for(auto imod : *unit->Imports) {
			try {
				SG::Module& imports = symbolTable.GetModule(imod->Name->Name);

				for(auto isym : *imod->Symbols) {
					try {
						SG::Symbol& symbol = imports.GetSymbol(isym->OriginalName);
						AddModuleImport(mod, isym->ImportName, &symbol, *isym, report);
					}
					catch(...) {
						report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ImportUndefinedSymbol, Diagnostics::ErrorLevel::Error,
								"ResolveImports", boost::str(boost::format("module \'%s\' does not contain symbol \'%s\'") % imod->Name->Name % isym->OriginalName),
								isym->Location));
						AddModuleImportError(mod, isym->ImportName, *isym, report);
					}
				}
			}
			catch(...) {
				report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ImportUndefinedModule, Diagnostics::ErrorLevel::Error,
						"ResolveImports", boost::str(boost::format("module \'%s\' does not exist") % imod->Name->Name), imod->Location));

				for(auto isym : *imod->Symbols) {
					AddModuleImportError(mod, isym->ImportName, *isym, report);
				}
			}
		}
	}
}

}
}
}
}
