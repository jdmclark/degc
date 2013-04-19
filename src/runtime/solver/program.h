#pragma once

#include "network.h"
#include "runtime/math/set.h"
#include "runtime/solver/recordindex.h"
#include "runtime/vm/virtualmachine.h"
#include <vector>

namespace Deg {
namespace Runtime {
namespace Solver {

class ProgramNetwork {
private:
	size_t record_type;
	std::vector<Math::Set> source_sets;
	std::vector<Math::Set> limit_sets;
	std::vector<std::vector<size_t>> limit_subsets;
	Network network;

public:
	ProgramNetwork(size_t record_type, const std::vector<Math::Set>& source_sets, const std::vector<Math::Set>& limit_sets,
			const std::vector<std::vector<size_t>>& limit_subsets, const Network& network);

	bool Solve(const RecordIndex& recordIndex, NetworkSolver& ns) const;
};

class ProgramNetworkBranch {
public:
	std::vector<ProgramNetwork> chunks;
	std::vector<int> exclusion_sets;

	ProgramNetworkBranch(const std::vector<ProgramNetwork>& chunks, const std::vector<int>& exclusion_sets);
};

class ProgramNetworkReified {
public:
	std::vector<ProgramNetworkBranch> branches;
	std::vector<int> parameters;
	size_t code_ptr;

	bool SolveOne(const std::vector<ProgramNetwork>& chunks, const RecordIndex& recordIndex, NetworkSolver& ns) const;

	ProgramNetworkReified(const std::vector<ProgramNetworkBranch>& branches, const std::vector<int>& parameters, size_t code_ptr);

	bool Solve(const RecordIndex& recordIndex, VM::VirtualMachine& vm, NetworkSolver& ns) const;
};

class Program {
private:
	std::vector<std::unique_ptr<ProgramNetworkReified>> choices;
public:
	void AddReifiedProgram(std::unique_ptr<ProgramNetworkReified>& prog);
	bool Solve(const RecordIndex& recordIndex, VM::VirtualMachine& vm, NetworkSolver& ns, const std::vector<int>& parameters = {}) const;
};

}
}
}
