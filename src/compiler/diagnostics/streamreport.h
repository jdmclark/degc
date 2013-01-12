#pragma once

#include "report.h"
#include <iosfwd>

namespace Deg {
namespace Compiler {
namespace Diagnostics {

class StreamReport : public Report {
private:
	unsigned int errorCount, warningCount;
	std::ostream& str;

public:
	StreamReport(std::ostream& stream);

	void AddError(const Error&);

	unsigned int GetErrorCount() const;
	unsigned int GetWarningCount() const;
};

}
}
}
