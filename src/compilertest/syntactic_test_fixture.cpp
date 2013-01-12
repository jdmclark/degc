#include "syntactic_test_fixture.h"
#include "compiler/stages/stages.h"
#include <fstream>

SyntacticTestFixture::SyntacticTestFixture(const boost::filesystem::path& BasePath)
	: BasePath(BasePath) {
	return;
}

void SyntacticTestFixture::ParseFile(const boost::filesystem::path& filename) {
	Deg::Compiler::AST::Factory astFactory;
	Deg::Compiler::Stages::GenerateAST::GenerateAST(BasePath / filename, astFactory, Report);
}

void SyntacticTestFixture::PrintErrors() const {
	for(const auto& error : Report) {
		NullUnit::Test_Reporter->CaseExpectationFail(
				NullUnit::Test_Suite_Name,
				NullUnit::Test_Case_Name,
				static_cast<std::string>(error),
				error.Location.filename ? error.Location.filename : "internal",
				error.Location.first_line);
	}
}
