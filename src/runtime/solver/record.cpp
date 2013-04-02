#include "record.h"

Deg::Runtime::Solver::Record::Record(const std::vector<Math::DefaultFixed>& values)
	: values(values) {
	return;
}

std::ostream& Deg::Runtime::Solver::operator<<(std::ostream& os, const Record& r) {
	os << "[ ";
	for(auto v : r.values) {
		os << v << " ";
	}
	return os << "]";
}
