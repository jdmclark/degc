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

Case(CodomainMismatch) {
	ParseFile("codomain_mismatch.deg");
	AssertResult(0, 0);
}

Case(MeaninglessQuantity) {
	ParseFile("meaningless_quantity.deg");
	AssertResult(0, 1);
}

EndSuite(FunctionSemanticTest);
