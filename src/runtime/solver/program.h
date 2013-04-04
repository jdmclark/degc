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
	std::vector<std::vector<Math::Set>> limit_sets;
	Network network;

public:
	ProgramNetwork(size_t record_type, const std::vector<Math::Set>& source_sets, const std::vector<std::vector<Math::Set>>& limit_sets, const Network& network);

	bool Solve(RecordIndex& recordIndex, NetworkSolver& ns) const;
};

class Program {
private:
	std::vector<ProgramNetwork> networks;

public:
	Program(const std::vector<ProgramNetwork>& networks);

	bool Solve(RecordIndex& recordIndex, NetworkSolver& ns) const;
};

}
}
}
