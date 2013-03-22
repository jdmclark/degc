#include "functiontable.h"

void Deg::Runtime::Code::FunctionTable::AddFunction(const std::string& fn_name, size_t fn_offset) {
	function_offsets.insert(std::make_pair(fn_name, fn_offset));
}

size_t Deg::Runtime::Code::FunctionTable::GetFunction(const std::string& fn_name) const {
	auto it = function_offsets.find(fn_name);
	if(it == function_offsets.end()) {
		throw std::exception();
	}

	return it->second;
}
