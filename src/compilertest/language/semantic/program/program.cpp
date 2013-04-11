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

Case(ArgCountMismatch) {
	ParseFile("arg_count_mismatch.deg");
	AssertResult(1, 0);
}

Case(ArgNotEnum) {
	ParseFile("arg_not_enum.deg");
	AssertResult(1, 0);
}

Case(ArgTypeMismatch) {
	ParseFile("arg_type_mismatch.deg");
	AssertResult(1, 0);
}

Case(AssertNotBoolean) {
	ParseFile("assert_not_boolean.deg");
	AssertResult(1, 0);
}

Case(BaseUndefined) {
	ParseFile("base_undefined.deg");
	AssertResult(1, 0);
}

Case(EmbedNotProgram) {
	ParseFile("embed_not_program.deg");
	AssertResult(1, 0);
}

Case(ForAllEmbed) {
	ParseFile("for_all_embed.deg");
	AssertResult(1, 0);
}

Case(ForAllLimit) {
	ParseFile("for_all_limit.deg");
	AssertResult(2, 0);
}

Case(ForAllSetTypeMismatch) {
	ParseFile("for_all_set_type_mismatch.deg");
	AssertResult(1, 0);
}

Case(ForAllTake) {
	ParseFile("for_all_take.deg");
	AssertResult(2, 0);
}

Case(ForAnySetTypeMismatch) {
	ParseFile("for_any_set_type_mismatch.deg");
	AssertResult(1, 0);
}

Case(ForBestPredicateMismatch) {
	ParseFile("for_best_predicate_mismatch.deg");
	AssertResult(1, 0);
}

Case(ForBestSetTypeMismatch) {
	ParseFile("for_best_set_type_mismatch.deg");
	AssertResult(1, 0);
}

Case(IfElsePredicateNotBool) {
	ParseFile("if_else_predicate_not_bool.deg");
	AssertResult(1, 0);
}

Case(IfPredicateNotBool) {
	ParseFile("if_predicate_not_bool.deg");
	AssertResult(1, 0);
}

Case(LimitConst) {
	ParseFile("limit_const.deg");
	AssertResult(0, 0);
}

Case(LimitHasNoQuantity) {
	ParseFile("limit_has_no_quantity.deg");
	AssertResult(1, 0);
}

Case(LimitNotConst) {
	ParseFile("limit_not_const.deg");
	AssertResult(1, 0);
}

Case(LimitNotNumber) {
	ParseFile("limit_not_number.deg");
	AssertResult(1, 0);
}

Case(LimitOnLoopValue) {
	ParseFile("limit_on_loop_value.deg");
	AssertResult(1, 0);
}

Case(LimitSetNotConst) {
	ParseFile("limit_set_not_const.deg");
	AssertResult(1, 0);
}

Case(NameRedefinition) {
	ParseFile("name_redefinition.deg");
	AssertResult(1, 0);
}

Case(TakeConst) {
	ParseFile("take_const.deg");
	AssertResult(0, 0);
}

Case(TakeHasNoQuantity) {
	ParseFile("take_has_no_quantity.deg");
	AssertResult(1, 0);
}

Case(TakeNotConst) {
	ParseFile("take_not_const.deg");
	AssertResult(1, 0);
}

Case(TakeNotNumber) {
	ParseFile("take_not_number.deg");
	AssertResult(1, 0);
}

Case(TakeOnLoopValue) {
	ParseFile("take_on_loop_value.deg");
	AssertResult(1, 0);
}

Case(TakeSetNotConst) {
	ParseFile("take_set_not_const.deg");
	AssertResult(1, 0);
}

EndSuite(ProgramSemanticTest);
