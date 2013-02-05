#include "errorlocation.h"
#include <cstring>

Deg::Compiler::Diagnostics::ErrorLocation::ErrorLocation()
	: filename(nullptr), first_line(1), first_column(1), last_line(1), last_column(1) {
	return;
}

Deg::Compiler::Diagnostics::ErrorLocation::ErrorLocation(const char* filename,
		unsigned int first_line, unsigned int first_column, unsigned int last_line, unsigned int last_column)
	: filename(filename), first_line(first_line), first_column(first_column), last_line(last_line), last_column(last_column) {
	return;
}

Deg::Compiler::Diagnostics::ErrorLocation::ErrorLocation(const ErrorLocation& loc) {
	*this = loc;
}

const Deg::Compiler::Diagnostics::ErrorLocation& Deg::Compiler::Diagnostics::ErrorLocation::operator=(const ErrorLocation& loc) {
	filename = loc.filename;
	first_line = loc.first_line;
	first_column = loc.first_column;
	last_line = loc.last_line;
	last_column = loc.last_column;
	return loc;
}

bool Deg::Compiler::Diagnostics::ErrorLocation::operator==(const ErrorLocation& l) const {
	return std::strcmp(filename, l.filename) == 0 && first_line == l.first_line && last_line == l.last_line
			&& first_column == l.first_column && last_column == l.last_column;
}
