#include "scope.h"

Deg::Compiler::SG::Node& Deg::Compiler::SG::Scope::GetMember(const std::string& name) const {
	auto it = children_map.find(name);
	if(it == children_map.end()) {
		throw std::exception();
	}

	return *it->second;
}
