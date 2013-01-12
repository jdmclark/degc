#include "compiler/stages/stages.h"
#include "compiler/grammar/instance.h"
#include <fstream>
#include <boost/format.hpp>

Deg::Compiler::AST::TranslationUnit* Deg::Compiler::Stages::GenerateAST::GenerateAST(const boost::filesystem::path& filename,
		AST::Factory& Factory, Diagnostics::Report& Report) {
	std::ifstream file(filename.native());
	if(!file) {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FileNotFound, Diagnostics::ErrorLevel::CriticalError,
				"GenerateAST", boost::str(boost::format("could not open file: %s") % filename.native())));
		return nullptr;
	}

	Grammar::Instance inst(file, filename.native(), Factory, Report);
	return inst.Parse();
}
