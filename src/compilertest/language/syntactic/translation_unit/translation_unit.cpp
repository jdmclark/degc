#include <nullunit/nullunit.h>
#include "compilertest/syntactic_test_fixture.h"

class TranslationUnitSyntacticTestFixture : public SyntacticTestFixture {
public:
	TranslationUnitSyntacticTestFixture()
		: SyntacticTestFixture("src/compilertest/language/syntactic/translation_unit") {
		return;
	}
};

BeginSuiteFixture(TranslationUnitSyntacticTest, TranslationUnitSyntacticTestFixture);

Case(Empty) {
	ParseFile("empty.deg");
	AssertResult(0, 0);
}

Case(FileDoesNotExist) {
	ParseFile("filedoesnotexist.deg");
	AssertResult(1, 0);
}

EndSuite(TranslationUnitSyntacticTest);
