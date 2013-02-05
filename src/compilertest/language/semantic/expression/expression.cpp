#include <nullunit/nullunit.h>
#include "compilertest/semantic_test_fixture.h"

class ExpressionSemanticTestFixture : public SemanticTestFixture {
public:
	ExpressionSemanticTestFixture()
		: SemanticTestFixture("src/compilertest/language/semantic/expression") {
		return;
	}
};

BeginSuiteFixture(ExpressionSemanticTest, ExpressionSemanticTestFixture);

Case(CheckAddOp) {
	ParseFiles({"check_base.deg", "check_add_op.deg"});
	AssertResult(14, 0);
}

Case(CheckSubOp) {
	ParseFiles({"check_base.deg", "check_sub_op.deg"});
	AssertResult(14, 0);
}

Case(EnumDoesNotExist) {
	ParseFile("enum_does_not_exist.deg");
	AssertResult(2, 0);
}

Case(EnumMemberDoesNotExist) {
	ParseFile("enum_member_does_not_exist.deg");
	AssertResult(1, 0);
}

Case(FunctionDoesNotExist) {
	ParseFile("function_does_not_exist.deg");
	AssertResult(0, 0);
}

Case(FunctionInsufficientArgs) {
	ParseFile("function_insufficient_args.deg");
	AssertResult(0, 0);
}

Case(FunctionTooManyArgs) {
	ParseFile("function_too_many_args.deg");
	AssertResult(0, 0);
}

EndSuite(ExpressionSemanticTest);
