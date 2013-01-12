#pragma once

#include <iosfwd>

namespace Deg {
namespace Compiler {
namespace Diagnostics {

enum class ErrorCode {
	// Internal system errors
	Internal				= 1000,
	FeatureNotImplemented	= 1001,
	FileNotFound			= 1002,

	// Tokenizer errors
	UnrecognizedInput		= 2000,

	// Syntax errors
	SyntaxError				= 3000,
};

std::ostream& operator<<(std::ostream&, ErrorCode);

}
}
}
