#include "program_visitor.h"
#include "program_set_visitor.h"
#include "program_const_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::ProgramVisitor;
using Deg::Compiler::Stages::GenerateCode::ProgramNetworkChunk;
using Deg::Compiler::Stages::GenerateCode::ProgramNetworkSetChunk;
using namespace Deg::Runtime::Math;

ProgramNetworkSetChunk::ProgramNetworkSetChunk(const Set& set)
	: set(set) {
	return;
}

void ProgramNetworkChunk::AddRequirement(const Set& set, DefaultFixed qty) {
	size_t new_requirement = requirements.size();
	requirements.push_back(qty);

	Set current = set;
	std::vector<ProgramNetworkSetChunk> new_chunks;

	for(const auto& old_chunk : set_chunks) {
		Set left = old_chunk.set - current;
		if(!left.IsEmpty()) {
			ProgramNetworkSetChunk new_chunk(left);
			new_chunk.linked_limits = old_chunk.linked_limits;
			new_chunk.linked_requirements = old_chunk.linked_requirements;
			new_chunks.push_back(new_chunk);
		}

		Set middle = old_chunk.set & current;
		if(!middle.IsEmpty()) {
			ProgramNetworkSetChunk new_chunk(middle);
			new_chunk.linked_limits = old_chunk.linked_limits;
			new_chunk.linked_requirements = old_chunk.linked_requirements;
			new_chunk.linked_requirements.push_back(new_requirement);
			new_chunks.push_back(new_chunk);
		}

		current = current - old_chunk.set;
	}

	if(!current.IsEmpty()) {
		ProgramNetworkSetChunk new_chunk(current);
		new_chunk.linked_requirements.push_back(new_requirement);
		new_chunks.push_back(new_chunk);
	}

	set_chunks = new_chunks;
}

void ProgramNetworkChunk::AddLimit(const Set& set, DefaultFixed qty) {
	size_t new_limit = limits.size();
	limits.push_back(std::make_pair(qty, set));

	Set current = set;
	std::vector<ProgramNetworkSetChunk> new_chunks;

	for(const auto& old_chunk : set_chunks) {
		Set left = old_chunk.set - current;
		if(!left.IsEmpty()) {
			ProgramNetworkSetChunk new_chunk(left);
			new_chunk.linked_limits = old_chunk.linked_limits;
			new_chunk.linked_requirements = old_chunk.linked_requirements;
			new_chunks.push_back(new_chunk);
		}

		Set middle = old_chunk.set & current;
		if(!middle.IsEmpty()) {
			ProgramNetworkSetChunk new_chunk(middle);
			new_chunk.linked_limits = old_chunk.linked_limits;
			new_chunk.linked_requirements = old_chunk.linked_requirements;
			new_chunk.linked_limits.push_back(new_limit);
			new_chunks.push_back(new_chunk);
		}

		current = current - old_chunk.set;
	}

	if(!current.IsEmpty()) {
		ProgramNetworkSetChunk new_chunk(current);
		new_chunk.linked_limits.push_back(new_limit);
		new_chunks.push_back(new_chunk);
	}

	set_chunks = new_chunks;
}

ProgramVisitor::ProgramVisitor(Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::ProgramVisitor", report), recordTypeTable(recordTypeTable) {
	return;
}

void ProgramVisitor::AddRequirement(size_t record_type, const Set& set, DefaultFixed qty) {
	ProgramNetworkChunk* chunk = GetChunk(record_type);
	chunk->AddRequirement(set, qty);
}

void ProgramVisitor::AddLimit(size_t record_type, const Set& set, DefaultFixed qty) {
	ProgramNetworkChunk* chunk = GetChunk(record_type);
	chunk->AddLimit(set, qty);
}

void ProgramVisitor::VisitCompoundStatement(CompoundStatement& n) {
	for(auto& stmt : n.Statements) {
		stmt->Accept(*this);
	}
}

void ProgramVisitor::VisitTakeStatement(TakeStatement& n) {
	ProgramSetVisitor psv(recordTypeTable, Report);
	n.Set->Accept(psv);

	ProgramConstVisitor pcv(Report);
	n.Amount->Accept(pcv);

	AddRequirement(psv.result_record_type, psv.result, pcv.value);
}

void ProgramVisitor::VisitLimitStatement(LimitStatement& n) {
	ProgramSetVisitor psv(recordTypeTable, Report);
	n.Set->Accept(psv);

	ProgramConstVisitor pcv(Report);
	n.Amount->Accept(pcv);

	AddLimit(psv.result_record_type, psv.result, pcv.value);
}
