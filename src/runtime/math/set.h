#pragma once

#include "relation.h"
#include "interval.h"
#include "fixed.h"
#include <map>
#include <vector>
#include <iostream>

namespace Deg {
namespace Runtime {
namespace Math {

class Set {
public:
	struct Conjunction {
		std::map<unsigned int, Interval<DefaultFixed>> Clauses;

		Conjunction();
		Conjunction(unsigned int Field, DefaultFixed BottomEq, DefaultFixed TopNeq);
		bool IsEmpty() const;
		bool IsSubsetOf(const Conjunction&) const;

		inline std::map<unsigned int, Interval<DefaultFixed>>::const_iterator begin() const {
			return Clauses.begin();
		}

		inline std::map<unsigned int, Interval<DefaultFixed>>::const_iterator end() const {
			return Clauses.end();
		}
	};

private:
	std::vector<Conjunction> Disjunction;

	static void DisjoinOne(const Conjunction& base, const Conjunction& minus, std::vector<Conjunction>& output);
	void Disjoin();
	static bool CanMerge(const Conjunction& a, const Conjunction& b);
	static void MergeOne(Conjunction& target, const Conjunction& c);
	bool TryMerge();
	void Merge();
	static void ReduceOne(Conjunction& target);
	void Reduce();

public:
	Set();
	Set(unsigned int field, Relation relation, DefaultFixed value);

	bool IsEmpty() const;
	bool IsSubsetOf(const Set&) const;

	bool operator==(const Set&) const;
	bool operator!=(const Set&) const;

	Set operator&(const Set&) const;
	Set operator|(const Set&) const;
	Set operator-(const Set&) const;

	friend std::ostream& operator<<(std::ostream& os, const Set& s);

	inline std::vector<Conjunction>::const_iterator begin() const {
		return Disjunction.begin();
	}

	inline std::vector<Conjunction>::const_iterator end() const {
		return Disjunction.end();
	}
};

std::ostream& operator<<(std::ostream& os, const Set& s);

}
}
}
