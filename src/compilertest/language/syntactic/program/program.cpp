#include <nullunit/nullunit.h>
#include "compilertest/syntactic_test_fixture.h"

class ProgramSyntacticTestFixture : public SyntacticTestFixture {
public:
	ProgramSyntacticTestFixture()
		: SyntacticTestFixture("src/compilertest/language/syntactic/program") {
		return;
	}
};

BeginSuiteFixture(ProgramSyntacticTest, ProgramSyntacticTestFixture);

Case(Args) {
	ParseFile("args.deg");
	AssertResult(0, 0);
}

Case(Assert) {
	ParseFile("assert.deg");
	AssertResult(0, 0);
}

Case(EitherOr) {
	ParseFile("either_or.deg");
	AssertResult(0, 0);
}

Case(EmbedInline) {
	ParseFile("embed_inline.deg");
	AssertResult(0, 0);
}

Case(Embed) {
	ParseFile("embed.deg");
	AssertResult(0, 0);
}

Case(Empty) {
	ParseFile("empty.deg");
	AssertResult(0, 0);
}

Case(ExtendsArgs) {
	ParseFile("extends_args.deg");
	AssertResult(0, 0);
}

Case(Extends) {
	ParseFile("extends.deg");
	AssertResult(0, 0);
}

Case(ForAllInSet) {
	ParseFile("for_all_in_set.deg");
	AssertResult(0, 0);
}

Case(ForAll) {
	ParseFile("for_all.deg");
	AssertResult(0, 0);
}

Case(ForAnyInSet) {
	ParseFile("for_any_in_set.deg");
	AssertResult(0, 0);
}

Case(ForAny) {
	ParseFile("for_any.deg");
	AssertResult(0, 0);
}

Case(ForBestInSet) {
	ParseFile("for_best_in_set.deg");
	AssertResult(0, 0);
}

Case(ForBest) {
	ParseFile("for_best.deg");
	AssertResult(0, 0);
}

Case(IfElseIf) {
	ParseFile("if_else_if.deg");
	AssertResult(0, 0);
}

Case(IfElse) {
	ParseFile("if_else.deg");
	AssertResult(0, 0);
}

Case(If) {
	ParseFile("if.deg");
	AssertResult(0, 0);
}

Case(Limit) {
	ParseFile("limit.deg");
	AssertResult(0, 0);
}

Case(MultipleMember) {
	ParseFile("multiple_member.deg");
	AssertResult(0, 0);
}

Case(NamedMember) {
	ParseFile("named_member.deg");
	AssertResult(0, 0);
}

Case(SingleMember) {
	ParseFile("single_member.deg");
	AssertResult(0, 0);
}

Case(Take) {
	ParseFile("take.deg");
	AssertResult(0, 0);
}

EndSuite(ProgramSyntacticTest);
