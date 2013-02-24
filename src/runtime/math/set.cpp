#include "set.h"
#include <limits>
#include <algorithm>
#include <sstream>
#include <iostream>

Deg::Runtime::Math::Set::Conjunction::Conjunction() {
	return;
}

Deg::Runtime::Math::Set::Conjunction::Conjunction(unsigned int Field, int BottomEq, int TopNeq) {
	Clauses[Field] = Interval<int>(BottomEq, TopNeq);
	return;
}

bool Deg::Runtime::Math::Set::Conjunction::IsEmpty() const {
	for(const auto& em : Clauses) {
		if(em.second.IsEmpty()) {
			return true;
		}
	}

	return false;
}

bool Deg::Runtime::Math::Set::Conjunction::IsSubsetOf(const Conjunction& c) const {
	for(const auto& em : Clauses) {
		auto it = c.Clauses.find(em.first);
		if(it != c.Clauses.end()) {
			if(!it->second.Contains(em.second)) {
				return false;
			}
		}
	}

	return true;
}

void Deg::Runtime::Math::Set::DisjoinOne(const Conjunction& base, const Conjunction& minus, std::vector<Conjunction>& output) {
	Conjunction middle_conj = base;

	for(const auto& rng : minus.Clauses) {
		Interval<int> left, middle, right;

		auto it = middle_conj.Clauses.find(rng.first);
		if(it == middle_conj.Clauses.end()) {
			// Full coverage.
			left = Interval<int>(std::numeric_limits<int>::lowest(), rng.second.bottom);
			middle = Interval<int>(rng.second);
			right = Interval<int>(rng.second.top, std::numeric_limits<int>::max());
		}
		else {
			left = Interval<int>(it->second.bottom, rng.second.bottom);
			middle = Interval<int>(rng.second);
			right = Interval<int>(rng.second.top, it->second.top);
		}

		if(!left.IsEmpty()) {
			Conjunction left_conj(middle_conj);
			left_conj.Clauses[rng.first] = left;
			output.push_back(left_conj);
		}

		if(!right.IsEmpty()) {
			Conjunction right_conj(middle_conj);
			right_conj.Clauses[rng.first] = right;
			output.push_back(right_conj);
		}

		middle_conj.Clauses[rng.first] = middle;
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

bool Deg::Runtime::Math::Set::CanMerge(const Conjunction& a, const Conjunction& b) {
	bool union_once = false;
	for(const auto& rng : a.Clauses) {
		auto it = b.Clauses.find(rng.first);
		if(it != b.Clauses.end()) {
			if(rng.second == it->second) {
				continue;
			}
			else if(union_once) {
				return false;
			}
			else if(Interval<int>::CanUnion(rng.second, it->second)) {
				union_once = true;
			}
			else {
				return false;
			}
		}
	}

	return true;
}

void Deg::Runtime::Math::Set::MergeOne(Conjunction& target, const Conjunction& c) {
	for(auto& rng : target.Clauses) {
		auto it = c.Clauses.find(rng.first);
		if(it != c.Clauses.end()) {
			rng.second = Interval<int>::Union(rng.second, it->second);
		}
		else {
			rng.second = Interval<int>(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());
		}
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

void Deg::Runtime::Math::Set::ReduceOne(Conjunction& target) {
	for(auto it = target.Clauses.begin(); it != target.Clauses.end();) {
		if(it->second.bottom == std::numeric_limits<int>::lowest() && it->second.top == std::numeric_limits<int>::max()) {
			it = target.Clauses.erase(it);
		}
		else {
			++it;
		}
	}
}

void Deg::Runtime::Math::Set::Reduce() {
	Disjoin();
	Merge();

	for(auto& conj : Disjunction) {
		ReduceOne(conj);
	}

	auto it = std::remove_if(Disjunction.begin(), Disjunction.end(),
			[](const Conjunction& val) { return val.IsEmpty(); });
	Disjunction.erase(it, Disjunction.end());

	return;
}

Deg::Runtime::Math::Set::Set() {
	return;
}

Deg::Runtime::Math::Set::Set(unsigned int field, Relation relation, int value) {
	switch(relation) {
	case Relation::Less:
		Disjunction.push_back(Conjunction(field, std::numeric_limits<int>::lowest(), value));
		break;

	case Relation::LessEqual:
		Disjunction.push_back(Conjunction(field, std::numeric_limits<int>::lowest(), value + 1));
		break;

	case Relation::Greater:
		Disjunction.push_back(Conjunction(field, value + 1, std::numeric_limits<int>::max()));
		break;

	case Relation::GreaterEqual:
		Disjunction.push_back(Conjunction(field, value, std::numeric_limits<int>::max()));
		break;

	case Relation::Equal:
		Disjunction.push_back(Conjunction(field, value, value + 1));
		break;

	case Relation::NotEqual:
		Disjunction.push_back(Conjunction(field, std::numeric_limits<int>::lowest(), value));
		Disjunction.push_back(Conjunction(field, value + 1, std::numeric_limits<int>::max()));
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
	Set rv;
	for(const auto& cj : Disjunction) {
		for(const auto& scj : s.Disjunction) {
			Conjunction c = cj;
			for(const auto& sclause : scj.Clauses) {
				auto it = c.Clauses.find(sclause.first);
				if(it == c.Clauses.end()) {
					// No such clause
					c.Clauses[sclause.first] = sclause.second;
				}
				else {
					it->second = Interval<int>::Intersect(it->second, sclause.second);
				}
			}

			rv.Disjunction.push_back(c);
		}
	}

	rv.Reduce();
	return rv;
}

Deg::Runtime::Math::Set Deg::Runtime::Math::Set::operator|(const Set& s) const {
	Set rv;
	rv.Disjunction.insert(rv.Disjunction.end(), Disjunction.begin(), Disjunction.end());
	rv.Disjunction.insert(rv.Disjunction.end(), s.Disjunction.begin(), s.Disjunction.end());
	rv.Reduce();
	return rv;
}

Deg::Runtime::Math::Set Deg::Runtime::Math::Set::operator-(const Set& s) const {
	if(s.IsEmpty()) {
		return *this;
	}

	Set rv;

	std::vector<Conjunction> output;
	std::vector<Conjunction> current = Disjunction;

	for(const auto& dis : s.Disjunction) {
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
		for(const auto& con : dis.Clauses) {
			if(p_con_and) {
				ss << "^";
			}
			else {
				p_con_and = true;
			}

			ss << con.first << ":[" << con.second.bottom << "," << con.second.top << "]";
		}

		ss << ")";
	}

	ss << "}";
	return os << ss.str();
}
