#pragma once

#include "compiler/sg/visitor.h"
#include "runtime/math/set.h"
#include "runtime/solver/network.h"
#include "runtime/code/recordtypetable.h"
#include "compiler/ir/printer.h"
#include <unordered_map>
#include <memory>
#include <vector>

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateCode {

class ProgramNetworkSetChunk {
public:
	ProgramNetworkSetChunk(const Runtime::Math::Set& set);

	Runtime::Math::Set set;
	std::vector<size_t> linked_requirements;
	std::vector<size_t> linked_limits;
};

class ProgramNetworkChunk {
public:
	size_t record_type;
	std::vector<ProgramNetworkSetChunk> set_chunks;
	std::vector<Runtime::Math::DefaultFixed> requirements;
	std::vector<std::pair<Runtime::Math::DefaultFixed, Runtime::Math::Set>> limits;

	ProgramNetworkChunk(size_t record_type);

	void AddRequirement(const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
	void AddLimit(const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
};

class ProgramNetworkBranch {
private:
	inline ProgramNetworkChunk& GetChunk(size_t record_type) {
		for(size_t i = 0; i < chunks.size(); ++i) {
			if(chunks[i].record_type == record_type) {
				return chunks[i];
			}
		}

		chunks.emplace_back(record_type);
		return chunks.back();
	}

public:
	std::vector<ProgramNetworkChunk> chunks;
	std::vector<int> exclusion_sets;

	void AddRequirement(size_t record_type, const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
	void AddLimit(size_t record_type, const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
};

class ProgramVisitor : public SG::Visitor {
private:
	IR::Printer& code;
	Runtime::Code::RecordTypeTable& recordTypeTable;
	std::vector<SG::EnumerationMemberSymbol*> programArguments;
	int current_block;

public:
	std::vector<ProgramNetworkBranch> branches;

	inline void SetProgramArguments(const std::vector<SG::EnumerationMemberSymbol*>& args) {
		programArguments = args;
	}

	void AddRequirement(size_t record_type, const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
	void AddLimit(size_t record_type, const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);

	ProgramVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report);

	void VisitAssertStatement(SG::AssertStatement& n);
	void VisitCompoundStatement(SG::CompoundStatement& n);
	void VisitTakeStatement(SG::TakeStatement& n);
	void VisitLimitStatement(SG::LimitStatement& n);
	void VisitDisjunctionStatement(SG::DisjunctionStatement& n);
	void VisitIfStatement(SG::IfStatement& n);
	void VisitIfElseStatement(SG::IfElseStatement& n);
	void VisitEmbedStatement(SG::EmbedStatement& n);

	void EmbedProgram(SG::ProgramSymbol& n, const std::vector<std::unique_ptr<SG::Expression>>& args);
	void BeginBasicBlock();
	void EndBasicBlock();
};

}
}
}
}
