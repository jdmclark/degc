#include "storedreport.h"

Deg::Compiler::Diagnostics::StoredReport::StoredReport()
	: errorCount(0), warningCount(0) {
	return;
}

void Deg::Compiler::Diagnostics::StoredReport::AddError(const Error& e) {
	switch(e.Level) {
	case ErrorLevel::CriticalError:
	case ErrorLevel::Error:
		++errorCount;
		break;

	case ErrorLevel::Warning:
		++warningCount;
		break;
	}

	container.push_back(e);
}

unsigned int Deg::Compiler::Diagnostics::StoredReport::GetErrorCount() const {
	return errorCount;
}

unsigned int Deg::Compiler::Diagnostics::StoredReport::GetWarningCount() const {
	return warningCount;
}
