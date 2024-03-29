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

Case(ArgTypeMismatch) {
	ParseFile("arg_type_mismatch.deg");
	AssertResult(1, 0);
}

Case(ArgUnknownType) {
	ParseFile("arg_unknown_type.deg");
	AssertResult(1, 0);
}

Case(CodomainMismatch) {
	ParseFile("codomain_mismatch.deg");
	AssertResult(1, 0);
}

Case(IfElseNotBoolean) {
	ParseFile("if_else_not_boolean.deg");
	AssertResult(1, 0);
}

Case(MeaninglessQuantity) {
	ParseFile("meaningless_quantity.deg");
	AssertResult(0, 1);
}

Case(ReturnMismatch) {
	ParseFile("return_mismatch.deg");
	AssertResult(1, 0);
}

EndSuite(FunctionSemanticTest);
