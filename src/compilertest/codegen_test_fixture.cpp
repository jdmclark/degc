#include "codegen_test_fixture.h"
#include "compiler/sg/table.h"
#include "compiler/stages/stages.h"
#include "compiler/ir/codeprinter.h"
#include "compiler/ir/textprinter.h"
#include "compiler/ir/splitprinter.h"
#include <fstream>

CodegenTestFixture::CodegenTestFixture(const boost::filesystem::path& BasePath)
	: LanguageTestFixture(BasePath), vm(code, recordIndex) {
	return;
}

void CodegenTestFixture::ParseFiles(const std::vector<boost::filesystem::path>& files) {
	Deg::Compiler::AST::Factory astFactory;
	Deg::Compiler::SG::SymbolTable symbolTable;
	std::vector<Deg::Compiler::AST::TranslationUnit*> translation_units;

	for(auto& filename : files) {
		translation_units.push_back(Deg::Compiler::Stages::GenerateAST::GenerateAST(BasePath / filename, astFactory, Report));
	}

	if(Report.GetErrorCount() > 0) {
		return;
	}

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

	Deg::Compiler::IR::TextPrinter text_printer(code_text);
	Deg::Compiler::IR::CodePrinter code_printer(code, functionTable, programTable);
	Deg::Compiler::IR::SplitPrinter split_printer({&text_printer, &code_printer});

	Deg::Compiler::Stages::GenerateCode::GenerateCode(symbolTable, split_printer, recordTypeTable, functionTable, programTable, Report);
	if(Report.GetErrorCount() > 0) {
		return;
	}
}
