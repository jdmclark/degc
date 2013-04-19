#include "program.h"
#include <iostream>

using Deg::Runtime::Math::DefaultFixed;

Deg::Runtime::Solver::ProgramNetwork::ProgramNetwork(size_t record_type, const std::vector<Math::Set>& source_sets,
		const std::vector<Math::Set>& limit_sets, const std::vector<std::vector<size_t>>& limit_subsets, const Network& network)
	: record_type(record_type), source_sets(source_sets), limit_sets(limit_sets), limit_subsets(limit_subsets), network(network) {
	return;
}

bool Deg::Runtime::Solver::ProgramNetwork::Solve(const RecordIndex& recordIndex, NetworkSolver& ns) const {
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

Deg::Runtime::Solver::ProgramNetworkBranch::ProgramNetworkBranch(const std::vector<ProgramNetwork>& chunks, const std::vector<int>& exclusion_sets)
	: chunks(chunks), exclusion_sets(exclusion_sets) {
	return;
}

Deg::Runtime::Solver::ProgramNetworkReified::ProgramNetworkReified(const std::vector<ProgramNetworkBranch>& branches, const std::vector<int>& parameters, size_t code_ptr)
	: branches(branches), parameters(parameters), code_ptr(code_ptr) {
	return;
}

bool Deg::Runtime::Solver::ProgramNetworkReified::SolveOne(const std::vector<ProgramNetwork>& chunks, const RecordIndex& recordIndex, NetworkSolver& ns) const {
	for(const auto& chunk : chunks) {
		if(!chunk.Solve(recordIndex, ns)) {
			return false;
		}
	}

	return true;
}

bool Deg::Runtime::Solver::ProgramNetworkReified::Solve(const RecordIndex& recordIndex, VM::VirtualMachine& vm, NetworkSolver& ns) const {
	// Execute attached code, storing results in rejection list.
	std::set<int>* old_rejects = vm.GetRejectionList();
	std::set<int> rejects;
	vm.SetRejectionList(&rejects);
	vm.Call<bool>(code_ptr);
	vm.SetRejectionList(old_rejects); // Restore previous rejection list

	// Evaluate branches not in rejection list:
	for(const auto& branch : branches) {
		bool exec_br = true;

		for(int exc : branch.exclusion_sets) {
			if(rejects.count(exc) > 0) {
				exec_br = false;
				break;
			}
		}

		if(exec_br && SolveOne(branch.chunks, recordIndex, ns)) {
			return true;
		}
	}

	return false;
}

void Deg::Runtime::Solver::Program::AddReifiedProgram(std::unique_ptr<ProgramNetworkReified>& prog) {
	choices.push_back(std::move(prog));
}

bool Deg::Runtime::Solver::Program::Solve(const RecordIndex& recordIndex, VM::VirtualMachine& vm, NetworkSolver& ns, const std::vector<int>& params) const {
	// (Slow) locate reified program matching input parameters.
	for(const auto& rprog : choices) {
		if(rprog->parameters == params) {
			return rprog->Solve(recordIndex, vm, ns);
		}
	}

	return false;
}
