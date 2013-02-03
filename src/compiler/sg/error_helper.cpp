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

void Deg::Compiler::SG::ErrorHelper::UndefinedSymbol(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::UndefinedSymbol, Diagnostics::ErrorLevel::Error,
			stage, boost::str(boost::format("symbol \'%s\' undefined") % name), location));
}

void Deg::Compiler::SG::ErrorHelper::RecordNameExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::RecordNameExpected, Diagnostics::ErrorLevel::Error,
			stage, boost::str(boost::format("symbol \'%s\' is not a record name") % name), location));
}

void Deg::Compiler::SG::ErrorHelper::MeaninglessQuantity(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::MeaninglessQuantity, Diagnostics::ErrorLevel::Warning,
			stage, "quantity treated as number outside of record", location));
}
