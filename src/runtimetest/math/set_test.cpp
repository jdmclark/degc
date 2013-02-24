#include <nullunit/nullunit.h>
#include "runtime/math/set.h"
#include <iostream>

using namespace Deg::Runtime::Math;

BeginSuite(SetTest);

Case(Exists) {
	Set set;
}

Case(RelationConstructors) {
	Set a(0, Relation::Less, 5);
	Set b(1, Relation::LessEqual, 6);
	Set c(2, Relation::Greater, -1);
	Set d(3, Relation::GreaterEqual, 7);
	Set e(4, Relation::Equal, 183);
	Set f(5, Relation::NotEqual, 0);
}

Case(ComparisonReflexive) {
	Set a(0, Relation::Less, 5);
	Set b(0, Relation::Less, 5);

	Test_Assert_Eq(a, b);
	Test_Assert(!(a != b));
}

Case(UnionExists) {
	Set a(0, Relation::Less, 5);
	Set b(1, Relation::Less, 7);
	Set c = a | b;
}

Case(IntersectExists) {
	Set a(0, Relation::Less, 5);
	Set b(0, Relation::Less, 6);
	Set c = a & b;
}

Case(SetMinusExists) {
	Set a(0, Relation::Less, 5);
	Set b(0, Relation::Less, 7);
	Set c = a - b;
}

Case(UnionReduces) {
	Set a(0, Relation::Less, 5);
	Set b(0, Relation::Less, 6);
	Test_Expect_Eq((a | b), b);

	Set c(0, Relation::Greater, 10);
	Set d(0, Relation::Greater, 0);
	Test_Expect_Eq((c | d), d);

	Set e(0, Relation::LessEqual, 10);
	Set f(0, Relation::Less, 5);
	Test_Expect_Eq((e | f), e);

	Set g(0, Relation::GreaterEqual, 0);
	Set h(0, Relation::Greater, 5);
	Test_Expect_Eq((g | h), g);

	Set i(0, Relation::LessEqual, 5);
	Set j(0, Relation::LessEqual, 4);
	Test_Expect_Eq((i | j), i);

	Set k(0, Relation::GreaterEqual, 5);
	Set l(0, Relation::GreaterEqual, 7);
	Test_Expect_Eq((k | l), k);

	Set m(0, Relation::Equal, 3);
	Set n(0, Relation::Greater, 3);
	Test_Expect_Eq((m | n), Set(0, Relation::GreaterEqual, 3));

	Set o(0, Relation::Equal, 3);
	Set p(0, Relation::Less, 3);
	Test_Expect_Eq((o | p), Set(0, Relation::LessEqual, 3));

	Set q(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 10)
			& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 5));
	Set r(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 5)
				& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 10));

	Set s(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 5)
				& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 5));
	Set t(Set(0, Relation::Greater, 5) & Set(0, Relation::LessEqual, 10)
					& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 5));
	Set u(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 5)
					& Set(1, Relation::GreaterEqual, 5) & Set(1, Relation::LessEqual, 10));

	Test_Expect_Eq((q | r), (s | t | u));
}

Case(IntersectReduces) {
	Set a(0, Relation::Less, 5);
	Set b(0, Relation::Less, 6);
	Test_Expect_Eq((a & b), a);

	Set c(0, Relation::Greater, 10);
	Set d(0, Relation::Greater, 0);
	Test_Expect_Eq((c & d), c);

	Set e(0, Relation::LessEqual, 10);
	Set f(0, Relation::Less, 5);
	Test_Expect_Eq((e & f), f);

	Set g(0, Relation::GreaterEqual, 0);
	Set h(0, Relation::Greater, 5);
	Test_Expect_Eq((g & h), h);

	Set i(0, Relation::LessEqual, 5);
	Set j(0, Relation::LessEqual, 4);
	Test_Expect_Eq((i & j), j);

	Set k(0, Relation::GreaterEqual, 5);
	Set l(0, Relation::GreaterEqual, 7);
	Test_Expect_Eq((k & l), l);

	Set m(0, Relation::Equal, 3);
	Set n(0, Relation::Greater, 3);
	Test_Expect((m & n).IsEmpty());

	Set o(0, Relation::Equal, 3);
	Set p(0, Relation::Less, 3);
	Test_Expect((o & p).IsEmpty());

	Set q(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 10)
			& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 5));
	Set r(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 5)
			& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 10));

	Set s(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 5)
			& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 5));

	Test_Expect_Eq((q & r), s);
}

Case(SetMinusReduces) {
	Set a(0, Relation::Less, 5);
	Set b(0, Relation::Less, 6);
	Test_Expect((a - b).IsEmpty());
	Test_Expect_Eq((b - a), Set(0, Relation::Equal, 5));

	Set c(0, Relation::Greater, 10);
	Test_Expect_Eq((a - c), a);
	Test_Expect_Eq((c - a), c);

	Set q(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 10)
			& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 5));
	Set r(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 5)
			& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 10));

	Set t = (q | r) - ((q - r) | (r - q));

	Set s(Set(0, Relation::GreaterEqual, 0) & Set(0, Relation::LessEqual, 5)
				& Set(1, Relation::GreaterEqual, 0) & Set(1, Relation::LessEqual, 5));

	Test_Expect_Eq(t, s);
}

EndSuite(SetTest);

