#include "error_helper.h"
#include <boost/format.hpp>

void Deg::Compiler::SG::ErrorHelper::SymbolRedefinition(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& symbol) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SymbolRedefinition, Diagnostics::ErrorLevel::Error,
			stage, boost::str(boost::format("symbol \'%s\' redefinition") % symbol), location));
}

void Deg::Compiler::SG::ErrorHelper::UndefinedTypename(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::UndefinedTypename, Diagnostics::ErrorLevel::Error,
			stage, boost::str(boost::format("unknown type \'%s\'") % name), location));
}
