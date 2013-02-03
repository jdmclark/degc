#include <nullunit/nullunit.h>
#include "compilertest/semantic_test_fixture.h"

class ProgramSemanticTestFixture : public SemanticTestFixture {
public:
	ProgramSemanticTestFixture()
		: SemanticTestFixture("src/compilertest/language/semantic/program") {
		return;
	}
};

BeginSuiteFixture(ProgramSemanticTest, ProgramSemanticTestFixture);

Case(BaseUndefined) {
	ParseFile("base_undefined.deg");
	AssertResult(1, 0);
}

EndSuite(ProgramSemanticTest);
