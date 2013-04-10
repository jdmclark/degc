#include "program.h"
#include <iostream>

using Deg::Runtime::Math::DefaultFixed;

Deg::Runtime::Solver::ProgramNetwork::ProgramNetwork(size_t record_type, const std::vector<Math::Set>& source_sets,
		const std::vector<Math::Set>& limit_sets, const std::vector<std::vector<size_t>>& limit_subsets, const Network& network)
	: record_type(record_type), source_sets(source_sets), limit_sets(limit_sets), limit_subsets(limit_subsets), network(network) {
	return;
}

bool Deg::Runtime::Solver::ProgramNetwork::Solve(RecordIndex& recordIndex, NetworkSolver& ns) const {
	std::vector<DefaultFixed> sources(source_sets.size(), DefaultFixed(0));
	std::vector<DefaultFixed> limits(limit_sets.size(), DefaultFixed(0));

	RecordTable* rt = recordIndex.GetRecordTable(record_type);
	if(rt) {
		for(size_t i = 0; i < source_sets.size(); ++i) {
			sources[i] = rt->QuantityOf(source_sets[i]);
		}

		for(size_t i = 0; i < limit_sets.size(); ++i) {
			limits[i] = rt->QuantityOf(limit_sets[i]);
		}
	}

	return ns.Solve(network, sources, limits, limit_subsets);
}

Deg::Runtime::Solver::Program::Program(const std::vector<ProgramNetwork>& networks)
	: networks(networks) {
	return;
}

bool Deg::Runtime::Solver::Program::Solve(RecordIndex& recordIndex, NetworkSolver& ns) const {
	for(const auto& network : networks) {
		if(!network.Solve(recordIndex, ns)) {
			return false;
		}
	}

	return true;
}
