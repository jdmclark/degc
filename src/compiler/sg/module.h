#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include "node.h"

namespace Deg {
namespace Compiler {
namespace SG {

class Module {
private:
	std::vector<std::pair<std::string, std::unique_ptr<Symbol>>> symbols;
	std::unordered_map<std::string, Symbol*> symbolmap;

public:
	template <typename T> T& MakeSymbol(const std::string& name) {
		auto it = symbolmap.find(name);
		if(it != symbolmap.end()) {
			throw std::exception();
		}

		T* sym = new T();
		symbols.push_back(std::make_pair(name, std::unique_ptr<T>(sym)));
		symbolmap.insert(std::make_pair(name, sym));
		return *sym;
	}

	inline Symbol& GetSymbol(const std::string& name) const {
		auto it = symbolmap.find(name);
		if(it == symbolmap.end()) {
			throw std::exception();
		}

		return *it->second;
	}

	inline void ImportSymbol(const std::string& name, Symbol* symbol) {
		auto it = symbolmap.find(name);
		if(it != symbolmap.end()) {
			throw std::exception();
		}

		symbolmap.insert(std::make_pair(name, symbol));
	}

	inline std::vector<std::pair<std::string, std::unique_ptr<Symbol>>>::const_iterator begin() const {
		return symbols.begin();
	}

	inline std::vector<std::pair<std::string, std::unique_ptr<Symbol>>>::const_iterator end() const {
		return symbols.end();
	}
};

}
}
}
