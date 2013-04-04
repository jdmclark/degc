#pragma once

#include <unordered_map>
#include <string>

namespace Deg {
namespace Runtime {
namespace Code {

class RecordTypeInfo {
public:
	size_t type_id, width, quantity;

	RecordTypeInfo(size_t type_id, size_t width, size_t quantity);
};

class RecordTypeTable {
private:
	std::unordered_map<std::string, RecordTypeInfo> record_types;

public:
	inline void AddRecordType(const std::string& name, size_t width, size_t quantity) {
		record_types.insert(std::make_pair(name, RecordTypeInfo(record_types.size(), width, quantity)));
	}

	inline RecordTypeInfo GetRecordType(const std::string& name) const {
		auto it = record_types.find(name);
		if(it == record_types.end()) {
			throw std::exception();
		}

		return it->second;
	}
};

}
}
}
