#include <iostream>
#include "errorlevel.h"

std::ostream& Deg::Compiler::Diagnostics::operator<<(std::ostream& str, ErrorLevel value) {
	switch(value) {
	case ErrorLevel::Warning:
		return str << "warning";

	case ErrorLevel::Error:
		return str << "error";

	case ErrorLevel::CriticalError:
		return str << "critical error";
	}

	return str;
}
