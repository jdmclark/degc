#include <nullunit/nullunit.h>
#include "compilertest/syntactic_test_fixture.h"

class ExpressionSyntacticTestFixture : public SyntacticTestFixture {
public:
	ExpressionSyntacticTestFixture()
		: SyntacticTestFixture("src/compilertest/language/syntactic/expression") {
		return;
	}
};

BeginSuiteFixture(ExpressionSyntacticTest, ExpressionSyntacticTestFixture);

Case(Arithmetic) {
	ParseFile("arithmetic.deg");
	AssertResult(0, 0);
}

Case(Comparison) {
	ParseFile("comparison.deg");
	AssertResult(0, 0);
}

Case(Contradiction) {
	ParseFile("contradiction.deg");
	AssertResult(0, 0);
}

Case(FunctionCall) {
	ParseFile("function_call.deg");
	AssertResult(0, 0);
}

Case(Identifier) {
	ParseFile("identifier.deg");
	AssertResult(0, 0);
}

Case(Logical) {
	ParseFile("logical.deg");
	AssertResult(0, 0);
}

Case(Numeric) {
	ParseFile("numeric.deg");
	AssertResult(0, 0);
}

Case(Parenthesized) {
	ParseFile("parenthesized.deg");
	AssertResult(0, 0);
}

Case(Set) {
	ParseFile("set.deg");
	AssertResult(0, 0);
}

Case(Tautology) {
	ParseFile("tautology.deg");
	AssertResult(0, 0);
}

EndSuite(ExpressionSyntacticTest);
