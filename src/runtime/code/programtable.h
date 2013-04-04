#pragma once

#include "runtime/solver/program.h"
#include <unordered_map>
#include <string>
#include <memory>

namespace Deg {
namespace Runtime {
namespace Code {

class ProgramTable {
private:
	std::unordered_map<std::string, std::unique_ptr<Solver::Program>> programs;

public:
	inline void AddProgram(const std::string& name, std::unique_ptr<Solver::Program>& program) {
		programs.insert(std::make_pair(name, std::move(program)));
	}

	inline Solver::Program* GetProgram(const std::string& name) const {
		auto it = programs.find(name);
		if(it == programs.end()) {
			throw std::exception();
		}

		return it->second.get();
	}
};

}
}
}
