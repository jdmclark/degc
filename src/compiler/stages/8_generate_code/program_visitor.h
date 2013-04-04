#pragma once

#include "compiler/sg/visitor.h"
#include "runtime/math/set.h"
#include "runtime/solver/network.h"
#include "runtime/code/recordtypetable.h"
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
	std::vector<ProgramNetworkSetChunk> set_chunks;
	std::vector<Runtime::Math::DefaultFixed> requirements;
	std::vector<Runtime::Math::DefaultFixed> limits;

	void AddRequirement(const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
	void AddLimit(const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
};

class ProgramVisitor : public SG::Visitor {
private:
	Runtime::Code::RecordTypeTable& recordTypeTable;

public:
	std::unordered_map<size_t, std::unique_ptr<ProgramNetworkChunk>> chunks;

	inline ProgramNetworkChunk* GetChunk(size_t record_type) {
		auto it = chunks.find(record_type);
		if(it == chunks.end()) {
			ProgramNetworkChunk* new_chk = new ProgramNetworkChunk();
			chunks.insert(std::make_pair(record_type, std::unique_ptr<ProgramNetworkChunk>(new_chk)));
			return new_chk;
		}

		return it->second.get();
	}

	void AddRequirement(size_t record_type, const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);
	void AddLimit(size_t record_type, const Runtime::Math::Set& set, Runtime::Math::DefaultFixed qty);

	ProgramVisitor(Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report);

	void VisitCompoundStatement(SG::CompoundStatement& n);
	void VisitTakeStatement(SG::TakeStatement& n);
	void VisitLimitStatement(SG::LimitStatement& n);
};

}
}
}
}
