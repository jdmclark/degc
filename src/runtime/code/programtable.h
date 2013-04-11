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
	std::vector<std::unique_ptr<Solver::Program>> programs;
	std::unordered_map<std::string, size_t> program_map;

public:
	inline size_t GetProgramId(const std::string& name) {
		auto it = program_map.find(name);
		if(it == program_map.end()) {
			// Reserve space
			program_map.insert(std::make_pair(name, programs.size()));
			programs.push_back(std::unique_ptr<Solver::Program>(nullptr));

			it = program_map.find(name);
		}

		return it->second;
	}

	inline void AddProgram(const std::string& name, std::unique_ptr<Solver::Program>& program) {
		size_t id = GetProgramId(name);
		programs[id] = std::move(program);
	}

	inline Solver::Program* GetProgram(const std::string& name) {
		return programs[GetProgramId(name)].get();
	}
};

}
}
}
