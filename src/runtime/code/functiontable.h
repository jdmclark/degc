#pragma once

#include <string>
#include <unordered_map>

namespace Deg {
namespace Runtime {
namespace Code {

class FunctionTable {
private:
	std::unordered_map<std::string, size_t> function_offsets;
	
public:
	void AddFunction(const std::string& fn_name, size_t offset);
	size_t GetFunction(const std::string& fn_name) const;
};

}
}
}
