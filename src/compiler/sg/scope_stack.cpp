#include "scope_stack.h"

Deg::Compiler::SG::ScopeStack::ScopeStack(SG::Module& module)
	: module(module) {
	return;
}

Deg::Compiler::SG::Node& Deg::Compiler::SG::ScopeStack::GetSymbol(const std::string& name) {
	for(auto it = scope_stack.rbegin(); it != scope_stack.rend(); ++it) {
		if((*it)->IsMember(name)) {
			return (*it)->GetMember(name);
		}
	}

	return module.GetSymbol(name);
}
