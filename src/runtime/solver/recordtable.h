#pragma once

#include <vector>
#include "runtime/math/fixed.h"
#include "runtime/math/set.h"

namespace Deg {
namespace Runtime {
namespace Solver {

class RecordTable {
protected:
	const size_t RecordWidth;
	const size_t QuantityIndex;

public:
	RecordTable(size_t RecordWidth, size_t QuantityIndex);
	virtual ~RecordTable();

	virtual void AddRecord(const std::vector<Math::DefaultFixed>& fields) = 0;
	virtual bool IsEmpty(const Math::Set& set) = 0;
	virtual Math::DefaultFixed QuantityOf(const Math::Set& set) = 0;
};

}
}
}
