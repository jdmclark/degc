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
	AssertResult(1, 0);
}

Case(FileDoesNotExist) {
	ParseFile("filedoesnotexist.deg");
	AssertResult(1, 0);
}

Case(ImportSymbolAs) {
	ParseFile("import_symbol_as.deg");
	AssertResult(0, 0);
}

Case(ImportSymbolList) {
	ParseFile("import_symbol_list.deg");
	AssertResult(0, 0);
}

Case(ImportSymbol) {
	ParseFile("import_symbol.deg");
	AssertResult(0, 0);
}

Case(ModuleDefinition) {
	ParseFile("module_definition.deg");
	AssertResult(0, 0);
}

EndSuite(TranslationUnitSyntacticTest);
