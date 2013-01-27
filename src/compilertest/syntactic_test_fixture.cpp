#include "syntactic_test_fixture.h"
#include "compiler/stages/stages.h"
#include <fstream>

SyntacticTestFixture::SyntacticTestFixture(const boost::filesystem::path& BasePath)
	: LanguageTestFixture(BasePath) {
	return;
}

void SyntacticTestFixture::ParseFiles(const std::vector<boost::filesystem::path>& files) {
	Deg::Compiler::AST::Factory astFactory;

	for(auto& filename : files) {
		Deg::Compiler::Stages::GenerateAST::GenerateAST(BasePath / filename, astFactory, Report);
	}
}
