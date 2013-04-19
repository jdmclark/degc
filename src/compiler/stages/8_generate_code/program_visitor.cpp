#include "program_visitor.h"
#include "program_set_visitor.h"
#include "program_const_visitor.h"
#include "expression_visitor.h"
#include "compiler/stages/6_constant_folding/expression_visitor.h"
#include <boost/format.hpp>
#include <iostream>

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::ProgramVisitor;
using Deg::Compiler::Stages::GenerateCode::ProgramNetworkBranch;
using Deg::Compiler::Stages::GenerateCode::ProgramNetworkChunk;
using Deg::Compiler::Stages::GenerateCode::ProgramNetworkSetChunk;
using namespace Deg::Runtime::Math;

ProgramNetworkSetChunk::ProgramNetworkSetChunk(const Set& set)
	: set(set) {
	return;
}

ProgramNetworkChunk::ProgramNetworkChunk(size_t record_type)
	: record_type(record_type) {
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

void ProgramNetworkBranch::AddRequirement(size_t record_type, const Set& set, DefaultFixed qty) {
	GetChunk(record_type).AddRequirement(set, qty);
}

void ProgramNetworkBranch::AddLimit(size_t record_type, const Set& set, DefaultFixed qty) {
	GetChunk(record_type).AddLimit(set, qty);
}

ProgramVisitor::ProgramVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::ProgramVisitor", report), code(code), recordTypeTable(recordTypeTable), current_block(0) {
	branches.push_back(ProgramNetworkBranch());
	return;
}

void ProgramVisitor::AddRequirement(size_t record_type, const Set& set, DefaultFixed qty) {
	for(auto& branch : branches) {
		branch.AddRequirement(record_type, set, qty);
	}
}

void ProgramVisitor::AddLimit(size_t record_type, const Set& set, DefaultFixed qty) {
	for(auto& branch : branches) {
		branch.AddLimit(record_type, set, qty);
	}
}

void ProgramVisitor::VisitAssertStatement(AssertStatement& n) {
	ExpressionVisitor ev(code, recordTypeTable, programArguments, Report);
	n.Value->Accept(ev);

	code.LAnd();
}

void ProgramVisitor::VisitCompoundStatement(CompoundStatement& n) {
	for(auto& stmt : n.Statements) {
		stmt->Accept(*this);
	}
}

void ProgramVisitor::VisitTakeStatement(TakeStatement& n) {
	ProgramSetVisitor psv(recordTypeTable, programArguments, Report);
	n.Set->Accept(psv);

	ProgramConstVisitor pcv(programArguments, Report);
	n.Amount->Accept(pcv);

	AddRequirement(psv.result_record_type, psv.result, pcv.value);
}

void ProgramVisitor::VisitLimitStatement(LimitStatement& n) {
	ProgramSetVisitor psv(recordTypeTable, programArguments, Report);
	n.Set->Accept(psv);

	ProgramConstVisitor pcv(programArguments, Report);
	n.Amount->Accept(pcv);

	AddLimit(psv.result_record_type, psv.result, pcv.value);
}

void ProgramVisitor::VisitDisjunctionStatement(DisjunctionStatement& n) {
	EndBasicBlock();

	std::vector<ProgramNetworkBranch> current_branches = branches;
	std::vector<ProgramNetworkBranch> next_branches;

	for(auto& stmt : n.Statements) {
		branches = current_branches;

		BeginBasicBlock();
		stmt->Accept(*this);
		EndBasicBlock();

		std::copy(branches.begin(), branches.end(), std::back_inserter(next_branches));
	}

	branches = next_branches;

	BeginBasicBlock();
}

void ProgramVisitor::VisitIfStatement(IfStatement& n) {
	EndBasicBlock();

	std::vector<ProgramNetworkBranch> current_branches = branches;

	// Encode 'if' block as either: assert [condition] or: pass
	BeginBasicBlock();

	ExpressionVisitor ev(code, recordTypeTable, programArguments, Report);
	n.Predicate->Accept(ev);
	code.LAnd();

	n.Code->Accept(*this);

	EndBasicBlock();

	std::copy(current_branches.begin(), current_branches.end(), std::back_inserter(branches));

	BeginBasicBlock();
}

void ProgramVisitor::VisitIfElseStatement(IfElseStatement& n) {
	EndBasicBlock();

	std::vector<ProgramNetworkBranch> current_branches = branches;
	std::vector<ProgramNetworkBranch> if_branches;

	// Encode 'if' block as either: assert [condition] or: [not condition]
	BeginBasicBlock();

	ExpressionVisitor ev(code, recordTypeTable, programArguments, Report);
	n.Predicate->Accept(ev);
	code.LAnd();

	n.Code->Accept(*this);

	EndBasicBlock();

	if_branches = branches;
	branches = current_branches;

	// Else block
	BeginBasicBlock();

	ExpressionVisitor eev(code, recordTypeTable, programArguments, Report);
	n.Predicate->Accept(eev);
	code.LNot();
	code.LAnd();

	n.ElseCode->Accept(*this);

	EndBasicBlock();

	std::copy(if_branches.begin(), if_branches.end(), std::back_inserter(branches));

	BeginBasicBlock();
}

void ProgramVisitor::VisitEmbedStatement(EmbedStatement& n) {
	ConstantFolding::ExpressionVisitor pcv({}, programArguments, Report);
	n.Value->Accept(pcv);

	SG::IdentifierExpression& ie = dynamic_cast<SG::IdentifierExpression&>(*pcv.GeneratedExpression);
	SG::ProgramSymbol& ps = dynamic_cast<SG::ProgramSymbol&>(*ie.ReferencedNode);

	EmbedProgram(ps, n.Arguments);
}

void ProgramVisitor::EmbedProgram(SG::ProgramSymbol& n, const std::vector<std::unique_ptr<SG::Expression>>& args) {
	// Generate new argument vector for embedding.
	std::vector<SG::EnumerationMemberSymbol*> baseArgs;

	if(n.Arguments.children_size() != args.size()) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ProgramArgCountMismatch, Diagnostics::ErrorLevel::Error,
				VisitorName, boost::str(boost::format("embedded program expected %d arguments, found %d") % n.Arguments.children_size() % args.size()), n.ast_program->Location));
	}

	std::vector<SG::EnumerationSymbol*> expectedTypes;
	for(auto& arg : n.Arguments) {
		expectedTypes.push_back(dynamic_cast<SG::EnumerationType&>(*dynamic_cast<SG::ProgramArgumentSymbol&>(*arg).InputType).ElementType);
	}

	auto jt = expectedTypes.begin();
	for(auto it = args.begin(); it != args.end() && jt != expectedTypes.end(); ++it, ++jt) {
		ConstantFolding::ExpressionVisitor v({}, programArguments, Report);
		(*it)->Accept(v);

		SG::IdentifierExpression& e = dynamic_cast<SG::IdentifierExpression&>(*v.GeneratedExpression);
		SG::EnumerationMemberSymbol& ems = dynamic_cast<SG::EnumerationMemberSymbol&>(*e.ReferencedNode);

		bool found = false;
		for(const auto& sym : (*jt)->Values) {
			if(sym.get() == &ems) {
				found = true;
				break;
			}
		}

		if(!found) {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ProgramArgTypeMismatch, Diagnostics::ErrorLevel::Error,
					VisitorName, "embedded program parameter type mismatch", n.ast_program->Location));
		}

		baseArgs.push_back(&ems);
	}

	std::vector<SG::EnumerationMemberSymbol*> oldParams = programArguments;
	SetProgramArguments(baseArgs);

	if(n.Base) {
		EmbedProgram(*n.Base, n.BaseArguments);
	}

	// Add current statements
	n.Statements->Accept(*this);

	SetProgramArguments(oldParams);
}

void ProgramVisitor::BeginBasicBlock() {
	code.ConstB(true);
}

void ProgramVisitor::EndBasicBlock() {
	for(auto& branch : branches) {
		branch.exclusion_sets.push_back(current_block);
	}

	code.Assert(current_block);
	++current_block;
}
