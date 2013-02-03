#include <nullunit/nullunit.h>
#include "runtime/math/fixed.h"

using Deg::Runtime::Math::Fixed;
using Deg::Runtime::Math::DefaultFixed;

BeginSuite(FixedTests);

Case(LoadFromString) {
	Fixed<int, 0> q1("1");
	Fixed<int, 0> r1(1);
	Test_Assert_Eq(q1, r1);

	Fixed<int, 0> q2("100.67");
	Fixed<int, 0> r2(100);
	Test_Assert_Eq(q1, r1);

	Fixed<int, 0> q3("-52");
	Fixed<int, 0> r3(-52);
	Test_Assert_Eq(q3, r3);

	Fixed<int, 1> q4("1");
	Fixed<int, 1> r4(10);
	Test_Assert_Eq(q4, r4);

	Fixed<int, 6> q5("120");
	Fixed<int, 6> r5("120.000000");
	Test_Assert_Eq(q5, r5);

	Fixed<int, 6> q6("120");
	Fixed<int, 6> r6("120.0000001");
	Test_Assert_Eq(q6, r6);

	Fixed<int, 6> q7("120");
	Fixed<int, 6> r7("120.000001");
	Test_Assert_Neq(q7, r7);

	Fixed<int, 4> q8("1");
	Fixed<int, 4> r8(10000);
	Test_Assert_Eq(q8, r8);
}

Case(Addition) {
	Fixed<int, 0> q1 = Fixed<int, 0>("0") + Fixed<int, 0>("0");
	Fixed<int, 0> r1("0");
	Test_Assert_Eq(q1, r1);

	Fixed<int, 4> q2 = Fixed<int, 4>("123.0001") + Fixed<int, 4>("234.0009");
	Fixed<int, 4> r2("357.001");
	Test_Assert_Eq(q2, r2);

	Fixed<int, 4> q3 = Fixed<int, 4>("123.0001") + Fixed<int, 4>("-123");
	Fixed<int, 4> r3("0.0001");
	Test_Assert_Eq(q3, r3);
}

Case(Subtraction) {
	Fixed<int, 0> q1 = Fixed<int, 0>("3") - Fixed<int, 0>("1");
	Fixed<int, 0> r1("2");
	Test_Assert_Eq(q1, r1);

	Fixed<int, 4> q2 = Fixed<int, 4>("123.0001") - Fixed<int, 4>("123");
	Fixed<int, 4> r2 = Fixed<int, 4>("0.0001");
	Test_Assert_Eq(q2, r2);
}

Case(Multiplication) {
	Fixed<int, 0> q1 = Fixed<int, 0>("3") * Fixed<int, 0>("2");
	Fixed<int, 0> r1("6");
	Test_Assert_Eq(q1, r1);

	Fixed<int, 4> q2 = Fixed<int, 4>("123") * Fixed<int, 4>("0.001");
	Fixed<int, 4> r2("0.123");
	Test_Assert_Eq(q2, r2);
}

Case(Division) {
	DefaultFixed acc;
	for(int i = 0; i < 26; ++i) {
		acc += DefaultFixed("4.0");
	}

	for(int i = 0; i < 4; ++i) {
		acc += DefaultFixed("3.7");
	}

	for(int i = 0; i < 3; ++i) {
		acc += DefaultFixed("3.3");
	}

	acc /= DefaultFixed("33");
	Test_Assert_Eq(acc, DefaultFixed("3.9"));

	DefaultFixed q1 = DefaultFixed("1") / DefaultFixed("3");
	DefaultFixed r1 = DefaultFixed("0.3333");
	Test_Assert_Eq(q1, r1);

	DefaultFixed q2 = DefaultFixed("2") / DefaultFixed("3");
	DefaultFixed r2 = DefaultFixed("0.6666");
	Test_Assert_Eq(q2, r2);
}

EndSuite(FixedTests);
