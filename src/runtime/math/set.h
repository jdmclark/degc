#pragma once

#include "relation.h"
#include "interval.h"
#include "fixed.h"
#include <vector>
#include <iostream>

namespace Deg {
namespace Runtime {
namespace Math {

class Set {
public:
	struct Conjunction {
		std::vector<Interval<DefaultFixed>> Clauses;

		Conjunction(size_t RecordWidth);
		Conjunction(size_t RecordWidth, unsigned int Field, DefaultFixed BottomEq, DefaultFixed TopNeq);
		bool IsEmpty() const;
		bool IsSubsetOf(const Conjunction&) const;
	};

private:
	size_t RecordWidth;
	std::vector<Conjunction> Disjunction;

	void DisjoinOne(const Conjunction& base, const Conjunction& minus, std::vector<Conjunction>& output) const;
	void Disjoin();
	bool CanMerge(const Conjunction& a, const Conjunction& b) const;
	void MergeOne(Conjunction& target, const Conjunction& c) const;
	bool TryMerge();
	void Merge();
	void Reduce();

public:
	explicit Set(size_t width);
	Set(size_t width, unsigned int field, Relation relation, DefaultFixed value);

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
