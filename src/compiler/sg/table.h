#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "module.h"

namespace Deg {
namespace Compiler {
namespace SG {

class SymbolTable {
private:
	std::vector<std::unique_ptr<Module>> modules;
	std::unordered_map<std::string, Module*> modulemap;

public:
	Module& CreateModule(const std::string& name);

	inline Module& GetModule(const std::string& name) const {
		auto it = modulemap.find(name);
		if(it == modulemap.end()) {
			throw std::exception();
		}

		return *it->second;
	}

	inline std::vector<std::unique_ptr<Module>>::const_iterator begin() const {
		return modules.begin();
	}

	inline std::vector<std::unique_ptr<Module>>::const_iterator end() const {
		return modules.end();
	}
};

}
}
}
