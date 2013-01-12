#pragma once

#include "error.h"

namespace Deg {
namespace Compiler {
namespace Diagnostics {

class Report {
public:
	virtual ~Report();

	virtual void AddError(const Error&) = 0;

	virtual unsigned int GetErrorCount() const = 0;
	virtual unsigned int GetWarningCount() const = 0;
};

}
}
}
