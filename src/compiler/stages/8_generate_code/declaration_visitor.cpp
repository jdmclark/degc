#include "declaration_visitor.h"
#include "expression_visitor.h"
#include "program_visitor.h"
#include "program_const_visitor.h"
#include "compiler/stages/6_constant_folding/expression_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::DeclarationVisitor;
using namespace Deg::Runtime::Math;

DeclarationVisitor::DeclarationVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Runtime::Code::FunctionTable& functionTable,
		Runtime::Code::ProgramTable& programTable, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::DeclarationVisitor", report), code(code), recordTypeTable(recordTypeTable), functionTable(functionTable), programTable(programTable) {
	return;
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	code.Function(n.UniversalUniqueName);
	ExpressionVisitor v(code, recordTypeTable, {}, Report);
	n.Code->Accept(v);
	code.Ret();
}

std::unique_ptr<Deg::Runtime::Solver::ProgramNetworkReified> DeclarationVisitor::ReifyProgram(SG::ProgramSymbol& n, const std::vector<int>& bad_params,
		const std::vector<SG::EnumerationSymbol*>& value_types) {

	// Generate params vector matching actual enumeration members to integer arguments.
	std::vector<SG::EnumerationMemberSymbol*> params;
	for(size_t i = 0; i < bad_params.size(); ++i) {
		// Find matching enumeration value.
		SG::EnumerationMemberSymbol* val = nullptr;
		for(auto& member : value_types[i]->Values) {
			SG::EnumerationMemberSymbol& ems = dynamic_cast<SG::EnumerationMemberSymbol&>(*member);
			if(ems.Value == bad_params[i]) {
				val = &ems;
			}
		}

		params.push_back(val);
	}

	// Add function label
	std::stringstream ss;
	ss << n.UniversalUniqueName;
	for(int i : bad_params) {
		ss << "@" << i;
	}
	std::string fn_label = ss.str();
	code.Function(fn_label);

	ProgramVisitor pv(code, recordTypeTable, Report);
	pv.SetProgramArguments(params);

	pv.BeginBasicBlock();

	// Embed base program
	if(n.Base) {
		pv.EmbedProgram(*n.Base, n.BaseArguments);
	}

	// Embed self program
	n.Statements->Accept(pv);

	// Close program code
	pv.EndBasicBlock();
	code.ConstB(true);
	code.Ret();

	std::vector<Runtime::Solver::ProgramNetworkBranch> branches;

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

		branches.emplace_back(networks, branch.exclusion_sets);
	}

	return std::unique_ptr<Runtime::Solver::ProgramNetworkReified>(new Runtime::Solver::ProgramNetworkReified(branches, bad_params,
			functionTable.GetFunction(fn_label)));
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	std::unique_ptr<Runtime::Solver::Program> new_prog(new Runtime::Solver::Program());

	if(n.Arguments.children_size() == 0) {
		std::unique_ptr<Runtime::Solver::ProgramNetworkReified> reified_program = ReifyProgram(n, {}, {});
		new_prog->AddReifiedProgram(reified_program);
	}
	else {
		// Generate cartesian product of parameters.
		std::vector<int> params(n.Arguments.children_size(), 0);
		std::vector<int> limit_values;
		std::vector<SG::EnumerationSymbol*> value_types;
		bool overflow = false;

		for(auto& arg : n.Arguments) {
			SG::ProgramArgumentSymbol& parg = dynamic_cast<SG::ProgramArgumentSymbol&>(*arg);
			SG::EnumerationType& et = dynamic_cast<SG::EnumerationType&>(*parg.InputType);
			limit_values.push_back(et.ElementType->Values.children_size());
			value_types.push_back(et.ElementType);
		}

		while(!overflow) {
			std::unique_ptr<Runtime::Solver::ProgramNetworkReified> reified_program = ReifyProgram(n, params, value_types);
			new_prog->AddReifiedProgram(reified_program);

			// Increment param vector
			for(size_t i = 0; i < params.size() && i < limit_values.size(); ++i) {
				overflow = false;

				++params[i];
				if(params[i] >= limit_values[i]) {
					params[i] = 0;
					overflow = true;
				}
				else {
					break;
				}
			}
		}
	}

	programTable.AddProgram(n.UniversalUniqueName, new_prog);
}
