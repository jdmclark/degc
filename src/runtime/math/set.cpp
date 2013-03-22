#include "set.h"
#include <limits>
#include <algorithm>
#include <sstream>
#include <iostream>

Deg::Runtime::Math::Set::Conjunction::Conjunction(size_t RecordWidth)
	: Clauses(RecordWidth, Interval<DefaultFixed>(std::numeric_limits<DefaultFixed>::lowest(), std::numeric_limits<DefaultFixed>::max())) {
	return;
}

Deg::Runtime::Math::Set::Conjunction::Conjunction(size_t RecordWidth, unsigned int Field, DefaultFixed BottomEq, DefaultFixed TopNeq)
	: Clauses(RecordWidth, Interval<DefaultFixed>(std::numeric_limits<DefaultFixed>::lowest(), std::numeric_limits<DefaultFixed>::max())) {
	Clauses[Field] = Interval<DefaultFixed>(BottomEq, TopNeq);
	return;
}

bool Deg::Runtime::Math::Set::Conjunction::IsEmpty() const {
	for(const auto& em : Clauses) {
		if(em.IsEmpty()) {
			return true;
		}
	}

	return false;
}

bool Deg::Runtime::Math::Set::Conjunction::IsSubsetOf(const Conjunction& c) const {
	for(size_t i = 0; i < Clauses.size(); ++i) {
		if(!c.Clauses[i].Contains(Clauses[i])) {
			return false;
		}
	}

	return true;
}

void Deg::Runtime::Math::Set::DisjoinOne(const Conjunction& base, const Conjunction& minus, std::vector<Conjunction>& output) const {
	Conjunction middle_conj = base;

	for(size_t i = 0; i < RecordWidth; ++i) {
		Interval<DefaultFixed> left, middle, right;

		if(Interval<DefaultFixed>::Disjoint(base.Clauses[i], minus.Clauses[i])) {
			output.push_back(middle_conj);
			return;
		}

		left = Interval<DefaultFixed>(base.Clauses[i].bottom, minus.Clauses[i].bottom);
		middle = Interval<DefaultFixed>(std::max(base.Clauses[i].bottom, minus.Clauses[i].bottom), std::min(base.Clauses[i].top, minus.Clauses[i].top));
		right = Interval<DefaultFixed>(minus.Clauses[i].top, base.Clauses[i].top);

		if(!left.IsEmpty()) {
			Conjunction left_conj(middle_conj);
			left_conj.Clauses[i] = left;
			output.push_back(left_conj);
		}

		if(!right.IsEmpty()) {
			Conjunction right_conj(middle_conj);
			right_conj.Clauses[i] = right;
			output.push_back(right_conj);
		}

		middle_conj.Clauses[i] = middle;
	}
}

void Deg::Runtime::Math::Set::Disjoin() {
	if(Disjunction.empty()) {
		return;
	}

	std::vector<Conjunction> output;
	std::vector<Conjunction> current = { Disjunction.front() };

	for(auto it = Disjunction.begin() + 1; it != Disjunction.end(); ++it) {
		output.clear();

		for(auto& one : current) {
			DisjoinOne(one, *it, output);
		}

		output.push_back(*it);

		std::swap(output, current);
	}

	std::swap(current, Disjunction);
}

bool Deg::Runtime::Math::Set::CanMerge(const Conjunction& a, const Conjunction& b) const {
	bool union_once = false;

	for(size_t i = 0; i < RecordWidth; ++i) {
		if(a.Clauses[i] == b.Clauses[i]) {
			continue;
		}
		else if(union_once) {
			return false;
		}
		else if(Interval<DefaultFixed>::CanUnion(a.Clauses[i], b.Clauses[i])) {
			union_once = true;
		}
		else {
			return false;
		}
	}

	return true;
}

void Deg::Runtime::Math::Set::MergeOne(Conjunction& target, const Conjunction& c) const {
	for(size_t i = 0; i < RecordWidth; ++i) {
		target.Clauses[i] = Interval<DefaultFixed>::Union(target.Clauses[i], c.Clauses[i]);
	}
}

bool Deg::Runtime::Math::Set::TryMerge() {
	for(size_t i = 0; i < Disjunction.size(); ++i) {
		for(size_t j = i + 1; j < Disjunction.size();) {
			if(CanMerge(Disjunction[i], Disjunction[j])) {
				MergeOne(Disjunction[i], Disjunction[j]);
				if(Disjunction.size() - 1 > j) {
					Disjunction[j] = Disjunction[Disjunction.size() - 1];
				}

				Disjunction.pop_back();
				return true;
			}
			else {
				++j;
			}
		}
	}

	return false;
}

void Deg::Runtime::Math::Set::Merge() {
	while(TryMerge());
}

void Deg::Runtime::Math::Set::Reduce() {
	Disjoin();
	Merge();

	auto it = std::remove_if(Disjunction.begin(), Disjunction.end(), [](const Conjunction& val) { return val.IsEmpty(); });
	Disjunction.erase(it, Disjunction.end());

	return;
}

Deg::Runtime::Math::Set::Set(size_t width)
	: RecordWidth(width) {
	return;
}

Deg::Runtime::Math::Set::Set(size_t width, unsigned int field, Relation relation, DefaultFixed value)
	: RecordWidth(width) {
	switch(relation) {
	case Relation::Less:
		Disjunction.emplace_back(RecordWidth, field, std::numeric_limits<DefaultFixed>::lowest(), value);
		break;

	case Relation::LessEqual:
		Disjunction.emplace_back(RecordWidth, field, std::numeric_limits<DefaultFixed>::lowest(), value + DefaultFixed(1));
		break;

	case Relation::Greater:
		Disjunction.emplace_back(RecordWidth, field, value + DefaultFixed(1), std::numeric_limits<DefaultFixed>::max());
		break;

	case Relation::GreaterEqual:
		Disjunction.emplace_back(RecordWidth, field, value, std::numeric_limits<DefaultFixed>::max());
		break;

	case Relation::Equal:
		Disjunction.emplace_back(RecordWidth, field, value, value + DefaultFixed(1));
		break;

	case Relation::NotEqual:
		Disjunction.emplace_back(RecordWidth, field, std::numeric_limits<DefaultFixed>::lowest(), value);
		Disjunction.emplace_back(RecordWidth, field, value + DefaultFixed(1), std::numeric_limits<DefaultFixed>::max());
		break;
	}
}

bool Deg::Runtime::Math::Set::IsEmpty() const {
	for(const auto& em : Disjunction) {
		if(!em.IsEmpty()) {
			return false;
		}
	}

	return true;
}

bool Deg::Runtime::Math::Set::IsSubsetOf(const Set& s) const {
	for(const auto& em : Disjunction) {
		// Each conjunction must be the subset of a conjunction in s.
		bool is_subset = false;

		for(const auto& en : s.Disjunction) {
			if(em.IsSubsetOf(en)) {
				is_subset = true;
				break;
			}
		}

		if(!is_subset) {
			return false;
		}
	}

	return true;
}

bool Deg::Runtime::Math::Set::operator==(const Set& s) const {
	return IsSubsetOf(s) && s.IsSubsetOf(*this);
}

bool Deg::Runtime::Math::Set::operator!=(const Set& s) const {
	return !(*this == s);
}

Deg::Runtime::Math::Set Deg::Runtime::Math::Set::operator&(const Set& s) const {
	// Intersection
	Set rv(std::max(RecordWidth, s.RecordWidth));
	for(const auto& cj : Disjunction) {
		for(const auto& scj : s.Disjunction) {
			Conjunction c = cj;
			bool IsEmpty = false;
			for(size_t i = 0; i < RecordWidth; ++i) {
				c.Clauses[i] = Interval<DefaultFixed>::Intersect(c.Clauses[i], scj.Clauses[i]);
				if(c.Clauses[i].IsEmpty()) {
					IsEmpty = true;
					break;
				}
			}

			if(!IsEmpty) {
				rv.Disjunction.push_back(c);
			}
		}
	}

	rv.Reduce();
	return rv;
}

Deg::Runtime::Math::Set Deg::Runtime::Math::Set::operator|(const Set& s) const {
	Set rv(std::max(RecordWidth, s.RecordWidth));
	rv.Disjunction.insert(rv.Disjunction.end(), Disjunction.begin(), Disjunction.end());
	rv.Disjunction.insert(rv.Disjunction.end(), s.Disjunction.begin(), s.Disjunction.end());
	rv.Reduce();
	return rv;
}

Deg::Runtime::Math::Set Deg::Runtime::Math::Set::operator-(const Set& s) const {
	if(s.IsEmpty()) {
		return *this;
	}

	Set rv(std::max(RecordWidth, s.RecordWidth));

	std::vector<Conjunction> output;
	std::vector<Conjunction> current = Disjunction;

	for(const auto& dis : s.Disjunction) {
		output.clear();

		for(const auto& one : current) {
			DisjoinOne(one, dis, output);
		}

		std::swap(output, current);
	}

	std::swap(rv.Disjunction, current);
	rv.Reduce();
	return rv;
}

std::ostream& Deg::Runtime::Math::operator<<(std::ostream& os, const Set& s) {
	std::stringstream ss;
	ss << "{";

	bool p_dis_or = false;
	for(const auto& dis : s.Disjunction) {
		if(p_dis_or) {
			ss << "v";
		}
		else {
			p_dis_or = true;
		}

		ss << "(";

		bool p_con_and = false;
		for(size_t i = 0; i < s.RecordWidth; ++i) {
			if(p_con_and) {
				ss << "^";
			}
			else {
				p_con_and = true;
			}

			ss << i << ":[" << static_cast<std::string>(dis.Clauses[i].bottom) << "," << static_cast<std::string>(dis.Clauses[i].top) << "]";
		}

		ss << ")";
	}

	ss << "}";
	return os << ss.str();
}
