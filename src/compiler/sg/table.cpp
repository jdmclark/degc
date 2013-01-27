#include "table.h"
#include "module.h"

Deg::Compiler::SG::Module& Deg::Compiler::SG::SymbolTable::CreateModule(const std::string& name) {
	auto it = modulemap.find(name);
	if(it != modulemap.end()) {
		return *it->second;
	}

	// Module does not exist and must be added.
	Module* mod = new Module();
	modules.push_back(std::unique_ptr<Module>(mod));
	modulemap.insert(std::make_pair(name, mod));

	// Populate new module with imports from root module.
	Module& root = GetModule("");
	for(const auto& it : root) {
		mod->ImportSymbol(it.first, it.second.get());
	}

	return *mod;
}
