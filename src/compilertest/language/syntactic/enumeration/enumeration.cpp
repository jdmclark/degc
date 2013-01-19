#include <nullunit/nullunit.h>
#include "compilertest/syntactic_test_fixture.h"

class EnumerationSyntacticTestFixture : public SyntacticTestFixture {
public:
	EnumerationSyntacticTestFixture()
		: SyntacticTestFixture("src/compilertest/language/syntactic/enumeration") {
		return;
	}
};

BeginSuiteFixture(EnumerationSyntacticTest, EnumerationSyntacticTestFixture);

Case(Empty) {
	ParseFile("empty.deg");
	AssertResult(0, 0);
}

Case(MultipleMember) {
	ParseFile("multiple_member.deg");
	AssertResult(0, 0);
}

Case(SingleMember) {
	ParseFile("single_member.deg");
	AssertResult(0, 0);
}

Case(Whitespace) {
	ParseFile("whitespace.deg");
	AssertResult(0, 0);
}

EndSuite(EnumerationSyntacticTest);
