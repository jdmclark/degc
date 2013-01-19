#include <nullunit/nullunit.h>
#include "compilertest/syntactic_test_fixture.h"

class FunctionSyntacticTestFixture : public SyntacticTestFixture {
public:
	FunctionSyntacticTestFixture()
		: SyntacticTestFixture("src/compilertest/language/syntactic/function") {
		return;
	}
};

BeginSuiteFixture(FunctionSyntacticTest, FunctionSyntacticTestFixture);

Case(ExpressionFunction) {
	ParseFile("expression_function.deg");
	AssertResult(0, 0);
}

Case(FunctionArgEmpty) {
	ParseFile("function_arg_empty.deg");
	AssertResult(0, 0);
}

Case(FunctionArgMultiple) {
	ParseFile("function_arg_multiple.deg");
	AssertResult(0, 0);
}

Case(FunctionArgSingle) {
	ParseFile("function_arg_single.deg");
	AssertResult(0, 0);
}

Case(IfElseIfFunction) {
	ParseFile("if_else_if_function.deg");
	AssertResult(0, 0);
}

Case(IfFunction) {
	ParseFile("if_function.deg");
	AssertResult(0, 0);
}

EndSuite(FunctionSyntacticTest);
