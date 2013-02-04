#include <nullunit/nullunit.h>
#include "runtime/math/interval.h"

using Deg::Runtime::Math::Interval;

BeginSuite(IntervalTest);

Case(ContainsNumber) {
	Interval<int> a(12, 13);
	Test_Assert(a.Contains(12));

	Interval<int> b(12, 12);
	Test_Assert(!b.Contains(12));
}

Case(IsEmpty) {
	Interval<int> a(0, 0);
	Test_Assert(a.IsEmpty());

	Interval<int> b(-1, 1);
	Test_Assert(!b.IsEmpty());

	Interval<int> c(12, 13);
	Test_Assert(!c.IsEmpty());
}

Case(ContainsInterval) {
	Interval<int> a1(12, 12);
	Interval<int> b1(96, 96);
	Test_Assert(a1.Contains(b1));
	Test_Assert(b1.Contains(a1));

	Interval<int> a2(12, 12);
	Interval<int> b2(12, 13);
	Test_Assert(!a2.Contains(b2));
	Test_Assert(b2.Contains(a2));

	Interval<int> a3(-4, 95);
	Interval<int> b3(-5, 97);
	Test_Assert(!a3.Contains(b3));
	Test_Assert(b3.Contains(a3));

	Interval<int> a4(0, 10);
	Interval<int> b4(0, 10);
	Test_Assert(a4.Contains(b4));
	Test_Assert(b4.Contains(a4));
}

Case(Disjoint) {
	Interval<int> a1(0, 0);
	Interval<int> b1(0, 0);
	Test_Assert(Interval<int>::Disjoint(a1, b1));
	Test_Assert(Interval<int>::Disjoint(b1, a1));

	Interval<int> a2(0, 0);
	Interval<int> b2(-100, 100);
	Test_Assert(Interval<int>::Disjoint(a2, b2));
	Test_Assert(Interval<int>::Disjoint(b2, a2));

	Interval<int> a3(0, 100);
	Interval<int> b3(100, 200);
	Test_Assert(Interval<int>::Disjoint(a3, b3));
	Test_Assert(Interval<int>::Disjoint(b3, a3));

	Interval<int> a4(0, 101);
	Interval<int> b4(100, 200);
	Test_Assert(!Interval<int>::Disjoint(a4, b4));
	Test_Assert(!Interval<int>::Disjoint(b4, a4));

	Interval<int> a5(10, 20);
	Interval<int> b5(5, 25);
	Test_Assert(!Interval<int>::Disjoint(a5, b5));
	Test_Assert(!Interval<int>::Disjoint(b5, a5));
}

Case(Intersect) {
	Interval<int> a1(-10, -5);
	Interval<int> b1(5, 10);
	Test_Assert(Interval<int>::Intersect(a1, b1).IsEmpty());
	Test_Assert(Interval<int>::Intersect(b1, a1).IsEmpty());

	Interval<int> a2(0, 5);
	Interval<int> b2(5, 10);
	Test_Assert(Interval<int>::Intersect(a2, b2).IsEmpty());
	Test_Assert(Interval<int>::Intersect(b2, a2).IsEmpty());

	Interval<int> a3(0, 7);
	Interval<int> b3(2, 5);
	Test_Assert_Eq(Interval<int>::Intersect(a3, b3), b3);
	Test_Assert_Eq(Interval<int>::Intersect(b3, a3), b3);

	Interval<int> a4(0, 7);
	Interval<int> b4(3, 10);
	Test_Assert_Eq(Interval<int>::Intersect(a4, b4), Interval<int>(3, 7));
	Test_Assert_Eq(Interval<int>::Intersect(b4, a4), Interval<int>(3, 7));
}

Case(CanUnion) {
	Interval<int> a1(-10, -5);
	Interval<int> b1(5, 10);
	Test_Assert(!Interval<int>::CanUnion(a1, b1));
	Test_Assert(!Interval<int>::CanUnion(b1, a1));

	Interval<int> a2(0, 5);
	Interval<int> b2(5, 10);
	Test_Assert(Interval<int>::CanUnion(a2, b2));
	Test_Assert(Interval<int>::CanUnion(b2, a2));

	Interval<int> a3(0, 7);
	Interval<int> b3(3, 10);
	Test_Assert(Interval<int>::CanUnion(a3, b3));
	Test_Assert(Interval<int>::CanUnion(b3, a3));

	Interval<int> a4(0, 10);
	Interval<int> b4(3, 7);
	Test_Assert(Interval<int>::CanUnion(a4, b4));
	Test_Assert(Interval<int>::CanUnion(b4, a4));

	Interval<int> a5(10, 10);
	Interval<int> b5(2, 2);
	Test_Assert(Interval<int>::CanUnion(a5, b5));
	Test_Assert(Interval<int>::CanUnion(b5, a5));

	Interval<int> a6(10, 10);
	Interval<int> b6(0, 6);
	Test_Assert(Interval<int>::CanUnion(a6, b6));
	Test_Assert(Interval<int>::CanUnion(b6, a6));
}

Case(Union) {
	Interval<int> a1(-10, -5);
	Interval<int> b1(5, 10);
	try {
		Interval<int>::Union(a1, b1);
		Test_Assert_Always("did not throw exception");
	}
	catch(...) {
		// Consume exception
	}

	try {
		Interval<int>::Union(b1, a1);
		Test_Assert_Always("did not throw exception");
	}
	catch(...) {
		// Consume exception
	}

	Interval<int> a2(0, 5);
	Interval<int> b2(5, 10);
	Test_Assert_Eq(Interval<int>::Union(a2, b2), Interval<int>(0, 10));
	Test_Assert_Eq(Interval<int>::Union(b2, a2), Interval<int>(0, 10));

	Interval<int> a3(0, 7);
	Interval<int> b3(3, 10);
	Test_Assert_Eq(Interval<int>::Union(a3, b3), Interval<int>(0, 10));
	Test_Assert_Eq(Interval<int>::Union(b3, a3), Interval<int>(0, 10));

	Interval<int> a4(0, 10);
	Interval<int> b4(3, 7);
	Test_Assert_Eq(Interval<int>::Union(a4, b4), Interval<int>(0, 10));
	Test_Assert_Eq(Interval<int>::Union(b4, a4), Interval<int>(0, 10));

	Interval<int> a5(0, 10);
	Interval<int> b5(99, 99);
	Test_Assert_Eq(Interval<int>::Union(a5, b5), Interval<int>(0, 10));
	Test_Assert_Eq(Interval<int>::Union(b5, a5), Interval<int>(0, 10));

	Interval<int> a6(0, 0);
	Interval<int> b6(10, 10);
	Test_Assert(Interval<int>::Union(a6, b6).IsEmpty());
	Test_Assert(Interval<int>::Union(b6, a6).IsEmpty());
}

EndSuite(IntervalTest);
