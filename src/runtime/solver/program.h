#pragma once

#include "network.h"
#include "runtime/math/set.h"
#include "runtime/solver/recordindex.h"
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

class ProgramNetworkReified {
public:
	std::vector<std::vector<ProgramNetwork>> branches;
	std::vector<int> parameters;

	bool SolveOne(const std::vector<ProgramNetwork>& chunks, const RecordIndex& recordIndex, NetworkSolver& ns) const;

	ProgramNetworkReified(const std::vector<std::vector<ProgramNetwork>>& branches, const std::vector<int>& parameters);

	bool Solve(const RecordIndex& recordIndex, NetworkSolver& ns) const;
};

class Program {
private:
	std::vector<std::unique_ptr<ProgramNetworkReified>> choices;
public:
	void AddReifiedProgram(std::unique_ptr<ProgramNetworkReified>& prog);
	bool Solve(const RecordIndex& recordIndex, NetworkSolver& ns, const std::vector<int>& parameters = {}) const;
};

}
}
}
