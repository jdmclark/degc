#pragma once

#include "runtime/math/fixed.h"
#include <vector>
#include <iosfwd>

namespace Deg {
namespace Runtime {
namespace Solver {

class Record {
	friend std::ostream& operator<<(std::ostream& os, const Record& r);
private:
	std::vector<Math::DefaultFixed> values;
public:
	Record(const std::vector<Math::DefaultFixed>& values);

	inline Math::DefaultFixed operator[](size_t i) const {
		return values[i];
	}
};

std::ostream& operator<<(std::ostream& os, const Record& r);

}
}
}
