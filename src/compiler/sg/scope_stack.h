#pragma once

#include "scope.h"
#include "module.h"
#include "node.h"
#include <string>
#include <vector>

namespace Deg {
namespace Compiler {
namespace SG {

class ScopeStack {
private:
	SG::Module& module;
	std::vector<Scope*> scope_stack;

public:
	ScopeStack(SG::Module& module);

	SG::Node& GetSymbol(const std::string& name);

	inline void PushScope(Scope& scope) {
		scope_stack.push_back(&scope);
	}

	inline void PopScope() {
		if(!scope_stack.empty()) {
			scope_stack.pop_back();
		}
	}
};

}
}
}
