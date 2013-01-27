#include "error_helper.h"
#include <boost/format.hpp>

void Deg::Compiler::SG::ErrorHelper::SymbolRedefinition(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& symbol) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SymbolRedefinition, Diagnostics::ErrorLevel::Error,
			"ResolveImports", boost::str(boost::format("symbol \'%s\' redefinition") % symbol), location));
}
