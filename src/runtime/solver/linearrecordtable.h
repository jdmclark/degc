#pragma once

#include "recordtable.h"
#include <vector>

namespace Deg {
namespace Runtime {
namespace Solver {

class LinearRecordTable : public RecordTable {
private:
	size_t record_count;
	std::vector<Math::DefaultFixed> records;

	bool internal_set_contains(std::vector<Math::DefaultFixed>::const_iterator it, const Math::Set& set) const;

public:
	LinearRecordTable(size_t RecordWidth, size_t QuantityIndex);

	void AddRecord(const std::vector<Math::DefaultFixed>& fields);
	bool IsEmpty(const Math::Set& set) const;
	Math::DefaultFixed QuantityOf(const Math::Set& set) const;
};

}
}
}
