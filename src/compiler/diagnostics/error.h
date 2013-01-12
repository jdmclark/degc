#pragma once

#include "errorcode.h"
#include "errorlevel.h"
#include "errorlocation.h"

#include <string>
#include <iosfwd>

namespace Deg {
namespace Compiler {
namespace Diagnostics {

class Error {
public:
	const ErrorCode Code;
	const ErrorLevel Level;
	const std::string Stage;
	const std::string Reason;
	const ErrorLocation Location;

	Error(ErrorCode Code, ErrorLevel Level, const std::string& Stage, const std::string& Reason);
	Error(ErrorCode Code, ErrorLevel Level, const std::string& Stage, const std::string& Reason, const ErrorLocation& Location);

	operator std::string() const;
};

std::ostream& operator<<(std::ostream& str, const Error& e);

}
}
}
