#pragma once

#include <limits>
#include <algorithm>

namespace Deg {
namespace Runtime {
namespace Math {

template <typename T> class Interval {
private:
	T bottom, top;

public:
	Interval() : bottom(0), top(0) {
		return;
	}

	Interval(const T& left, const T& right)
		: bottom(left), top(right) {
		if(top < bottom) {
			top = bottom;
		}
		return;
	}

	bool IsEmpty() const {
		return bottom == top;
	}

	bool Contains(const T& value) const {
		return bottom <= value && value < top;
	}

	bool Contains(const Interval& a) const {
		return a.IsEmpty() || (bottom <= a.bottom && a.top <= top);
	}

	static bool Disjoint(const Interval& a, const Interval& b) {
		return a.IsEmpty() || b.IsEmpty() || a.bottom >= b.top || a.top <= b.bottom;
	}

	static Interval Intersect(const Interval& a, const Interval& b) {
		if(Disjoint(a, b)) {
			return Interval();
		}
		else {
			return Interval(std::max(a.bottom, b.bottom), std::min(a.top, b.top));
		}
	}

	static bool CanUnion(const Interval& a, const Interval& b) {
		return a.IsEmpty() || b.IsEmpty() || (b.bottom <= a.bottom && a.bottom <= b.top) || (a.bottom <= b.bottom && b.bottom <= a.top);
	}

	static Interval Union(const Interval& a, const Interval& b) {
		if(!CanUnion(a, b)) {
			throw std::exception();
		}
		else if(a.IsEmpty()) {
			return b;
		}
		else if(b.IsEmpty()) {
			return a;
		}
		else {
			return Interval(std::min(a.bottom, b.bottom), std::max(a.top, b.top));
		}
	}

	bool operator==(const Interval& a) const {
		return bottom == a.bottom && top == a.top;
	}

	bool operator!=(const Interval& a) const {
		return bottom != a.bottom || top != a.top;
	}

	bool operator >(const Interval& a) const {
		return bottom >= a.top;
	}

	bool operator >=(const Interval& a) const {
		return bottom >= a.bottom;
	}

	bool operator <(const Interval& a) const {
		return top <= a.bottom;
	}

	bool operator <=(const Interval& a) const {
		return top <= a.top;
	}
};

}
}
}
