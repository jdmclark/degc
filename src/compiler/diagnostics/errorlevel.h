#pragma once

#include <iosfwd>

namespace Deg {
namespace Compiler {
namespace Diagnostics {

enum class ErrorLevel {
	Warning,
	Error,
	CriticalError
};

std::ostream& operator<<(std::ostream& str, ErrorLevel level);

}
}
}
