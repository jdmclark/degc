#include <nullunit/nullunit.h>
#include "compilertest/syntactic_test_fixture.h"

class CharacteristicSyntacticTestFixture : public SyntacticTestFixture {
public:
	CharacteristicSyntacticTestFixture()
		: SyntacticTestFixture("src/compilertest/language/syntactic/characteristic") {
		return;
	}
};

BeginSuiteFixture(CharacteristicSyntacticTest, CharacteristicSyntacticTestFixture);

Case(ChooseConcentrations) {
	ParseFile("choose_concentrations.deg");
	AssertResult(0, 0);
}

Case(HonorsComputingScience) {
	ParseFile("honors_computing_science.deg");
	AssertResult(0, 0);
}

EndSuite(CharacteristicSyntacticTest);
