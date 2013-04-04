#pragma once

#include "recordtable.h"
#include <unordered_map>
#include <memory>

namespace Deg {
namespace Runtime {
namespace Solver {

class RecordIndex {
private:
	std::unordered_map<size_t, std::unique_ptr<RecordTable>> tables;

public:
	inline void AddRecordTable(size_t type_id, std::unique_ptr<RecordTable>& table) {
		tables.insert(std::make_pair(type_id, std::move(table)));
	}

	inline RecordTable* GetRecordTable(size_t type_id) const {
		auto it = tables.find(type_id);
		if(it == tables.end()) {
			return nullptr;
		}
		else {
			return it->second.get();
		}
	}
};

}
}
}
