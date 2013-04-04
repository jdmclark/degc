#include "linearrecordtable.h"

Deg::Runtime::Solver::LinearRecordTable::LinearRecordTable(size_t RecordWidth, size_t QuantityIndex)
	: RecordTable(RecordWidth, QuantityIndex) {
	return;
}

void Deg::Runtime::Solver::LinearRecordTable::AddRecord(const std::vector<Math::DefaultFixed>& fields) {
	for(size_t i = 0; i < fields.size() && i < RecordWidth; ++i) {
		records.push_back(fields[i]);
	}

	for(size_t i = fields.size(); i < RecordWidth; ++i) {
		records.push_back(Math::DefaultFixed(0));
	}
}

bool Deg::Runtime::Solver::LinearRecordTable::internal_set_contains(std::vector<Math::DefaultFixed>::const_iterator values, const Math::Set& set) const {
	if(set.begin() == set.end()) {
		return true;
	}

	for(auto it = set.begin(); it != set.end(); ++it) {
		bool included = true;

		for(size_t j = 0; j < RecordWidth; ++j) {
			included &= it->Clauses[j].Contains(*(values + j));
			if(!included) {
				break;
			}
		}

		if(included) {
			return true;
		}
	}

	return false;
}

bool Deg::Runtime::Solver::LinearRecordTable::IsEmpty(const Math::Set& set) const {
	for(auto it = records.begin(); it != records.end(); it += RecordWidth) {
		if(internal_set_contains(it, set)) {
			return false;
		}
	}

	return true;
}

Deg::Runtime::Math::DefaultFixed Deg::Runtime::Solver::LinearRecordTable::QuantityOf(const Math::Set& set) const {
	Math::DefaultFixed accum(0);

	for(auto it = records.begin(); it != records.end(); it += RecordWidth) {
		if(internal_set_contains(it, set)) {
			accum += *(it + QuantityIndex);
		}
	}

	return accum;
}
