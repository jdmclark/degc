#pragma once

#include "relation.h"
#include "interval.h"
#include <map>
#include <vector>
#include <iostream>

namespace Deg {
namespace Runtime {
namespace Math {

class Set {
private:
	struct Conjunction {
		std::map<unsigned int, Interval<int>> Clauses;

		Conjunction();
		Conjunction(unsigned int Field, int BottomEq, int TopNeq);
		bool IsEmpty() const;
		bool IsSubsetOf(const Conjunction&) const;
	};

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
	Set(unsigned int field, Relation relation, int value);

	bool IsEmpty() const;
	bool IsSubsetOf(const Set&) const;

	bool operator==(const Set&) const;
	bool operator!=(const Set&) const;

	Set operator&(const Set&) const;
	Set operator|(const Set&) const;
	Set operator-(const Set&) const;

	friend std::ostream& operator<<(std::ostream& os, const Set& s);
};

std::ostream& operator<<(std::ostream& os, const Set& s);

}
}
}
