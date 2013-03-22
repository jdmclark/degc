#include <nullunit/nullunit.h>
#include "runtime/math/set.h"
#include <iostream>

using namespace Deg::Runtime::Math;

BeginSuite(SetTest);

Case(Exists) {
	Set set(0);
}

Case(RelationConstructors) {
	Set a(6, 0, Relation::Less, DefaultFixed("5"));
	Set b(6, 1, Relation::LessEqual, DefaultFixed("6"));
	Set c(6, 2, Relation::Greater, DefaultFixed("-1"));
	Set d(6, 3, Relation::GreaterEqual, DefaultFixed("7"));
	Set e(6, 4, Relation::Equal, DefaultFixed("183"));
	Set f(6, 5, Relation::NotEqual, DefaultFixed("0"));
}

Case(ComparisonReflexive) {
	Set a(1, 0, Relation::Less, DefaultFixed("5"));
	Set b(1, 0, Relation::Less, DefaultFixed("5"));

	Test_Assert_Eq(a, b);
	Test_Assert(!(a != b));
}

Case(UnionExists) {
	Set a(2, 0, Relation::Less, DefaultFixed("5"));
	Set b(2, 1, Relation::Less, DefaultFixed("7"));
	Set c = a | b;
}

Case(IntersectExists) {
	Set a(1, 0, Relation::Less, DefaultFixed("5"));
	Set b(1, 0, Relation::Less, DefaultFixed("6"));
	Set c = a & b;
}

Case(SetMinusExists) {
	Set a(1, 0, Relation::Less, DefaultFixed("5"));
	Set b(1, 0, Relation::Less, DefaultFixed("7"));
	Set c = a - b;
}

Case(UnionReduces) {
	Set a(1, 0, Relation::Less, DefaultFixed("5"));
	Set b(1, 0, Relation::Less, DefaultFixed("6"));
	Test_Expect_Eq((a | b), b);

	Set c(1, 0, Relation::Greater, DefaultFixed("10"));
	Set d(1, 0, Relation::Greater, DefaultFixed("0"));
	Test_Expect_Eq((c | d), d);

	Set e(1, 0, Relation::LessEqual, DefaultFixed("10"));
	Set f(1, 0, Relation::Less, DefaultFixed("5"));
	Test_Expect_Eq((e | f), e);

	Set g(1, 0, Relation::GreaterEqual, DefaultFixed("0"));
	Set h(1, 0, Relation::Greater, DefaultFixed("5"));
	Test_Expect_Eq((g | h), g);

	Set i(1, 0, Relation::LessEqual, DefaultFixed("5"));
	Set j(1, 0, Relation::LessEqual, DefaultFixed("4"));
	Test_Expect_Eq((i | j), i);

	Set k(1, 0, Relation::GreaterEqual, DefaultFixed("5"));
	Set l(1, 0, Relation::GreaterEqual, DefaultFixed("7"));
	Test_Expect_Eq((k | l), k);

	Set m(1, 0, Relation::Equal, DefaultFixed("3"));
	Set n(1, 0, Relation::Greater, DefaultFixed("3"));
	Test_Expect_Eq((m | n), Set(1, 0, Relation::GreaterEqual, DefaultFixed("3")));

	Set o(1, 0, Relation::Equal, DefaultFixed("3"));
	Set p(1, 0, Relation::Less, DefaultFixed("3"));
	Test_Expect_Eq((o | p), Set(1, 0, Relation::LessEqual, DefaultFixed("3")));

	Set q(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("10"))
			& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("5")));
	Set r(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("5"))
				& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("10")));

	Set s(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("5"))
				& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("5")));
	Set t(Set(2, 0, Relation::Greater, DefaultFixed("5")) & Set(2, 0, Relation::LessEqual, DefaultFixed("10"))
					& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("5")));
	Set u(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("5"))
					& Set(2, 1, Relation::GreaterEqual, DefaultFixed("5")) & Set(2, 1, Relation::LessEqual, DefaultFixed("10")));

	Test_Expect_Eq((q | r), (s | t | u));
}

Case(IntersectReduces) {
	Set a(1, 0, Relation::Less, DefaultFixed("5"));
	Set b(1, 0, Relation::Less, DefaultFixed("6"));
	Test_Expect_Eq((a & b), a);

	Set c(1, 0, Relation::Greater, DefaultFixed("10"));
	Set d(1, 0, Relation::Greater, DefaultFixed("0"));
	Test_Expect_Eq((c & d), c);

	Set e(1, 0, Relation::LessEqual, DefaultFixed("10"));
	Set f(1, 0, Relation::Less, DefaultFixed("5"));
	Test_Expect_Eq((e & f), f);

	Set g(1, 0, Relation::GreaterEqual, DefaultFixed("0"));
	Set h(1, 0, Relation::Greater, DefaultFixed("5"));
	Test_Expect_Eq((g & h), h);

	Set i(1, 0, Relation::LessEqual, DefaultFixed("5"));
	Set j(1, 0, Relation::LessEqual, DefaultFixed("4"));
	Test_Expect_Eq((i & j), j);

	Set k(1, 0, Relation::GreaterEqual, DefaultFixed("5"));
	Set l(1, 0, Relation::GreaterEqual, DefaultFixed("7"));
	Test_Expect_Eq((k & l), l);

	Set m(1, 0, Relation::Equal, DefaultFixed("3"));
	Set n(1, 0, Relation::Greater, DefaultFixed("3"));
	Test_Expect((m & n).IsEmpty());

	Set o(1, 0, Relation::Equal, DefaultFixed("3"));
	Set p(1, 0, Relation::Less, DefaultFixed("3"));
	Test_Expect((o & p).IsEmpty());

	Set q(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("10"))
			& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("5")));
	Set r(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("5"))
			& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("10")));

	Set s(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("5"))
			& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("5")));

	Test_Expect_Eq((q & r), s);
}

Case(SetMinusReduces) {
	Set a(1, 0, Relation::Less, DefaultFixed("5"));
	Set b(1, 0, Relation::Less, DefaultFixed("6"));
	Test_Expect((a - b).IsEmpty());
	Test_Expect_Eq((b - a), Set(1, 0, Relation::GreaterEqual, DefaultFixed("5")) & Set(1, 0, Relation::Less, DefaultFixed("6")));

	Set c(1, 0, Relation::Greater, DefaultFixed("10"));
	Test_Expect_Eq((a - c), a);
	Test_Expect_Eq((c - a), c);

	Set q(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("10"))
			& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("5")));
	Set r(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("5"))
			& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("10")));

	Set t = (q | r) - ((q - r) | (r - q));

	Set s(Set(2, 0, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 0, Relation::LessEqual, DefaultFixed("5"))
				& Set(2, 1, Relation::GreaterEqual, DefaultFixed("0")) & Set(2, 1, Relation::LessEqual, DefaultFixed("5")));

	Test_Expect_Eq(t, s);
}

EndSuite(SetTest);

