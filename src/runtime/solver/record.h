#pragma once

#include "runtime/math/fixed.h"
#include <vector>

namespace Deg {
namespace Runtime {
namespace Solver {

class Record {
private:
	std::vector<Math::DefaultFixed> values;
public:
	Record(const std::vector<Math::DefaultFixed>& values);

	inline Math::DefaultFixed operator[](size_t i) const {
		return values[i];
	}
};

}
}
}
