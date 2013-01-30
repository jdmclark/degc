#include <nullunit/nullunit.h>
#include "compilertest/semantic_test_fixture.h"

class FunctionSemanticTestFixture : public SemanticTestFixture {
public:
	FunctionSemanticTestFixture()
		: SemanticTestFixture("src/compilertest/language/semantic/function") {
		return;
	}
};

BeginSuiteFixture(FunctionSemanticTest, FunctionSemanticTestFixture);

Case(ArgRedefinition) {
	ParseFile("arg_redefinition.deg");
	AssertResult(1, 0);
}

Case(ArgUnknownType) {
	ParseFile("arg_unknown_type.deg");
	AssertResult(1, 0);
}

EndSuite(FunctionSemanticTest);
