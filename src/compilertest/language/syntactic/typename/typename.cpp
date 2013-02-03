#include <nullunit/nullunit.h>
#include "compilertest/syntactic_test_fixture.h"

class TypenameSyntacticTestFixture : public SyntacticTestFixture {
public:
	TypenameSyntacticTestFixture()
		: SyntacticTestFixture("src/compilertest/language/syntactic/typename") {
		return;
	}
};

BeginSuiteFixture(TypenameSyntacticTest, TypenameSyntacticTestFixture);

Case(ConstrainedSetType) {
	ParseFile("constrained_set_type.deg");
	AssertResult(0, 0);
}

Case(FunctionType) {
	ParseFile("function_type.deg");
	AssertResult(0, 0);
}

Case(NamedType) {
	ParseFile("named_type.deg");
	AssertResult(0, 0);
}

Case(SetType) {
	ParseFile("set_type.deg");
	AssertResult(0, 0);
}

EndSuite(TypenameSyntacticTest);
