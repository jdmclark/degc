#include "declaration_visitor.h"
#include "expression_visitor.h"
#include "program_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::DeclarationVisitor;
using namespace Deg::Runtime::Math;

DeclarationVisitor::DeclarationVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Runtime::Code::ProgramTable& programTable, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::DeclarationVisitor", report), code(code), recordTypeTable(recordTypeTable), programTable(programTable) {
	return;
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	code.Function(n.UniversalUniqueName);
	ExpressionVisitor v(code, recordTypeTable, Report);
	n.Code->Accept(v);
	code.Ret();
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	ProgramVisitor pv(recordTypeTable, Report);

	// Add parent statements to pv.
	if(n.Base) {
		n.Base->Accept(pv);
	}

	// Add current statements
	n.Statements->Accept(pv);

	std::vector<std::vector<Runtime::Solver::ProgramNetwork>> branches;

	for(const auto& branch : pv.branches) {
		std::vector<Runtime::Solver::ProgramNetwork> networks;

		for(const auto& chunk : branch.chunks) {
			std::vector<Set> source_sets;
			std::vector<Set> limit_sets;
			std::vector<std::vector<size_t>> limit_subsets(chunk.limits.size());
			Runtime::Solver::make_network net;

			net.AddSources(chunk.set_chunks.size());

			for(DefaultFixed i : chunk.requirements) {
				net.AddRequirement(i);
			}

			for(const std::pair<DefaultFixed, Set>& p : chunk.limits) {
				limit_sets.push_back(p.second);
				net.AddLimit(p.first);
			}

			// Detect non-disjoint, non-subset limits
			bool error_seen = false;
			for(size_t i = 0; i < limit_sets.size(); ++i) {
				for(size_t j = 0; j < limit_sets.size(); ++j) {
					Set ij_diff = limit_sets[i] & limit_sets[j];
					if(!ij_diff.IsEmpty() && !limit_sets[i].IsSubsetOf(ij_diff) && !limit_sets[j].IsSubsetOf(ij_diff) && !error_seen) {
						Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::LimitNotDisjoint,
								Diagnostics::ErrorLevel::Error, VisitorName, "program contains non-disjoint, non-subset limits",
								n.ast_program->Location));
						error_seen = true;
					}
				}
			}

			// Process limit subsets
			for(size_t i = 0; i < limit_sets.size(); ++i) {

				// Find smallest superset
				size_t sup = i;
				for(size_t j = 0; j < limit_sets.size(); ++j) {
					if(i == j) {
						continue;
					}

					if(limit_sets[i].IsSubsetOf(limit_sets[j])
							&& (sup == i || limit_sets[j].IsSubsetOf(limit_sets[sup]))) {
						sup = j;
					}
				}

				if(sup != i) {
					limit_subsets[sup].push_back(i);
				}
			}

			// Process sources
			for(const ProgramNetworkSetChunk& set_chunk : chunk.set_chunks) {
				size_t curr_src = source_sets.size();

				source_sets.push_back(set_chunk.set);

				for(size_t i : set_chunk.linked_requirements) {
					net.AddEdgeFromSourceToRequirement(curr_src, i);
				}

				if(!set_chunk.linked_limits.empty()) {
					// Only one outgoing limit edge is allowed. Find best.
					size_t best_lim = set_chunk.linked_limits[0];
					for(size_t i : set_chunk.linked_limits) {
						if(limit_sets[i].IsSubsetOf(limit_sets[best_lim])) {
							best_lim = i;
						}
					}

					net.AddEdgeFromSourceToLimit(curr_src, best_lim);
				}
			}

			networks.emplace_back(chunk.record_type, source_sets, limit_sets, limit_subsets, net);
		}

		branches.push_back(networks);
	}

	std::unique_ptr<Runtime::Solver::Program> new_prog(new Runtime::Solver::Program(branches));
	programTable.AddProgram(n.UniversalUniqueName, new_prog);
}
