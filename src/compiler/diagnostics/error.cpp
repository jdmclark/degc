#include "error.h"

#include <iostream>
#include <sstream>
#include <iomanip>

Deg::Compiler::Diagnostics::Error::Error(ErrorCode Code, ErrorLevel Level, const std::string& Stage, const std::string& Reason)
	: Code(Code), Level(Level), Stage(Stage), Reason(Reason) {
	return;
}

Deg::Compiler::Diagnostics::Error::Error(ErrorCode Code, ErrorLevel Level, const std::string& Stage, const std::string& Reason, const ErrorLocation& Location)
	: Code(Code), Level(Level), Stage(Stage), Reason(Reason), Location(Location) {
	return;
}

Deg::Compiler::Diagnostics::Error::operator std::string() const {
	std::stringstream ss;
	ss << *this;
	return ss.str();
}

std::ostream& Deg::Compiler::Diagnostics::operator<<(std::ostream& str, const Error& e) {
	if(e.Location.filename) {
		return str << e.Location.filename << "(" << e.Location.first_line << "," << e.Location.first_column << "): "
				<< e.Level << " " << e.Code << ": " << e.Reason << " [" << e.Stage << "]";
	}
	else {
		return str << e.Level << " " << e.Code << ": " << e.Reason << " [" << e.Stage << "]";
	}
}
