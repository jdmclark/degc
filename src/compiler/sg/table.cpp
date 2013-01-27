#include "table.h"
#include "module.h"

Deg::Compiler::SG::Module& Deg::Compiler::SG::SymbolTable::CreateModule(const std::string& name) {
	auto it = modulemap.find(name);
	if(it != modulemap.end()) {
		return *it->second;
	}

	modules.push_back(std::unique_ptr<Module>(new Module()));
	modulemap.insert(std::make_pair(name, modules.back().get()));
	return *modules.back();
}
