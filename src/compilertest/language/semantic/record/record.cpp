#include <nullunit/nullunit.h>
#include "compilertest/semantic_test_fixture.h"

class RecordSemanticTestFixture : public SemanticTestFixture {
public:
	RecordSemanticTestFixture()
		: SemanticTestFixture("src/compilertest/language/semantic/record") {
		return;
	}
};

BeginSuiteFixture(RecordSemanticTest, RecordSemanticTestFixture);

Case(RecordMemberRedefinition) {
	ParseFile("record_member_redefinition.deg");
	AssertResult(1, 0);
}

Case(RecordMemberUnknownType) {
	ParseFile("record_member_unknown_type.deg");
	AssertResult(1, 0);
}

Case(RecordMultiQuantity) {
	ParseFile("record_multi_quantity.deg");
	AssertResult(1, 0);
}

Case(RecordRedefinition) {
	ParseFile("record_redefinition.deg");
	AssertResult(1, 0);
}

Case(ValidRecord) {
	ParseFile("valid_record.deg");
	AssertResult(0, 0);
}

EndSuite(RecordSemanticTest);
