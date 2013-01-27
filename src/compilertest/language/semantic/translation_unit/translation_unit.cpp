#include <nullunit/nullunit.h>
#include "compilertest/semantic_test_fixture.h"

class TranslationUnitSemanticTestFixture : public SemanticTestFixture {
public:
	TranslationUnitSemanticTestFixture()
		: SemanticTestFixture("src/compilertest/language/semantic/translation_unit") {
		return;
	}
};

BeginSuiteFixture(TranslationUnitSemanticTest, TranslationUnitSemanticTestFixture);

Case(ImportModuleDoesNotExist) {
	ParseFile("import_module_does_not_exist.deg");
	AssertResult(1, 0);
}

Case(ImportSymbolDoesNotExist) {
	ParseFiles({"import_symbol_does_not_exist_1.deg", "import_symbol_does_not_exist_2.deg"});
	AssertResult(1, 0);
}

Case(ImportSymbolRedefinition) {
	ParseFiles({"import_symbol_redefinition_1.deg", "import_symbol_redefinition_2.deg"});
	AssertResult(1, 0);
}

Case(SplitModule) {
	ParseFiles({"split_module_1.deg", "split_module_2.deg"});
	AssertResult(0, 0);
}

Case(ValidImport) {
	ParseFiles({"valid_import_1.deg", "valid_import_2.deg"});
	AssertResult(0, 0);
}

EndSuite(TranslationUnitSemanticTest);
