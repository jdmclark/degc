#include "semantic_test_fixture.h"
#include "compiler/sg/table.h"
#include "compiler/stages/stages.h"
#include "runtime/code/recordtypetable.h"
#include <fstream>

SemanticTestFixture::SemanticTestFixture(const boost::filesystem::path& BasePath)
	: LanguageTestFixture(BasePath) {
	return;
}

void SemanticTestFixture::ParseFiles(const std::vector<boost::filesystem::path>& files) {
	Deg::Compiler::AST::Factory astFactory;
	Deg::Compiler::SG::SymbolTable symbolTable;
	std::vector<Deg::Compiler::AST::TranslationUnit*> translation_units;

	for(auto& filename : files) {
		translation_units.push_back(Deg::Compiler::Stages::GenerateAST::GenerateAST(BasePath / filename, astFactory, Report));
	}

	if(Report.GetErrorCount() > 0) {
		return;
	}

	Deg::Runtime::Code::RecordTypeTable recordTypeTable;

	Deg::Compiler::Stages::GenerateSG::GenerateSG(translation_units, symbolTable, Report);
	if(Report.GetErrorCount() > 0) {
		return;
	}

	Deg::Compiler::Stages::ResolveImports::ResolveImports(translation_units, symbolTable, Report);
	if(Report.GetErrorCount() > 0) {
		return;
	}

	Deg::Compiler::Stages::GenerateMembers::GenerateMembers(symbolTable, recordTypeTable, Report);
	if(Report.GetErrorCount() > 0) {
		return;
	}

	Deg::Compiler::Stages::GenerateExpressions::GenerateExpressions(symbolTable, Report);
	if(Report.GetErrorCount() > 0) {
		return;
	}

	Deg::Compiler::Stages::ConstantFolding::ConstantFolding(symbolTable, Report);
	if(Report.GetErrorCount() > 0) {
		return;
	}

	Deg::Compiler::Stages::GenerateSetExpressions::GenerateSetExpressions(symbolTable, Report);
	if(Report.GetErrorCount() > 0) {
		return;
	}
}
