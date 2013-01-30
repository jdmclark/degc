#include <nullunit/nullunit.h>
#include "compilertest/semantic_test_fixture.h"

class EnumerationSemanticTestFixture : public SemanticTestFixture {
public:
	EnumerationSemanticTestFixture()
		: SemanticTestFixture("src/compilertest/language/semantic/enumeration") {
		return;
	}
};

BeginSuiteFixture(EnumerationSemanticTest, EnumerationSemanticTestFixture);

Case(EnumMemberRedefinition) {
	ParseFile("enum_member_redefinition.deg");
	AssertResult(1, 0);
}

EndSuite(EnumerationSemanticTest);
