#include "streamreport.h"
#include <iostream>

Deg::Compiler::Diagnostics::StreamReport::StreamReport(std::ostream& str)
	: errorCount(0), warningCount(0), str(str) {
	return;
}

void Deg::Compiler::Diagnostics::StreamReport::AddError(const Error& e) {
	switch(e.Level) {
	case ErrorLevel::CriticalError:
	case ErrorLevel::Error:
		++errorCount;
		break;

	case ErrorLevel::Warning:
		++warningCount;
		break;
	}

	str << e << std::endl;
}

unsigned int Deg::Compiler::Diagnostics::StreamReport::GetErrorCount() const {
	return errorCount;
}

unsigned int Deg::Compiler::Diagnostics::StreamReport::GetWarningCount() const {
	return warningCount;
}
