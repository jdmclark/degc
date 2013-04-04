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

	// TODO: Generate and output program networks
	std::vector<Runtime::Solver::ProgramNetwork> networks;

	for(const auto& chunk : pv.chunks) {
		std::vector<Set> source_sets;
		std::vector<std::vector<Set>> limit_sets(chunk.second->limits.size());
		Runtime::Solver::make_network net;

		net.AddSources(chunk.second->set_chunks.size());

		for(DefaultFixed i : chunk.second->requirements) {
			net.AddRequirement(i);
		}

		for(DefaultFixed i : chunk.second->limits) {
			net.AddLimit(i);
		}

		// Process sources
		for(ProgramNetworkSetChunk& set_chunk : chunk.second->set_chunks) {
			size_t curr_src = source_sets.size();

			source_sets.push_back(set_chunk.set);

			for(size_t i : set_chunk.linked_requirements) {
				net.AddEdgeFromSourceToRequirement(curr_src, i);
			}

			for(size_t i : set_chunk.linked_limits) {
				net.AddEdgeFromSourceToLimit(curr_src, i);

				limit_sets[i].push_back(set_chunk.set);
			}

			// TODO: Handle non-disjoint limits
		}

		networks.emplace_back(chunk.first, source_sets, limit_sets, net);
	}

	std::unique_ptr<Runtime::Solver::Program> new_prog(new Runtime::Solver::Program(networks));
	programTable.AddProgram(n.UniversalUniqueName, new_prog);
}
