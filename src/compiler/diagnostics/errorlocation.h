#pragma once

#include <string>

namespace Deg {
namespace Compiler {
namespace Diagnostics {

class ErrorLocation {
public:
	const char* filename;
	unsigned int first_line, first_column, last_line, last_column;

	ErrorLocation();
	ErrorLocation(const char* filename, unsigned int first_line, unsigned int first_column, unsigned int last_line, unsigned int last_column);
	ErrorLocation(const ErrorLocation& loc);

	const ErrorLocation& operator=(const ErrorLocation& loc);
	bool operator==(const ErrorLocation&) const;
};

}
}
}
