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

void Deg::Compiler::SG::ErrorHelper::BooleanExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::BooleanExpected, Diagnostics::ErrorLevel::Error,
			stage, "boolean expression expected", location));
}

void Deg::Compiler::SG::ErrorHelper::NumberExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::NumberExpected, Diagnostics::ErrorLevel::Error,
			stage, "number expression expected", location));
}

void Deg::Compiler::SG::ErrorHelper::SetExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SetExpected, Diagnostics::ErrorLevel::Error,
			stage, "set expression expected", location));
}

void Deg::Compiler::SG::ErrorHelper::SetDomainMismatch(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SetDomainMismatch, Diagnostics::ErrorLevel::Error,
			stage, "set domain mismatch", location));
}

void Deg::Compiler::SG::ErrorHelper::SymbolNotExpression(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SymbolNotExpression, Diagnostics::ErrorLevel::Error,
			stage, boost::str(boost::format("symbol \'%s\' is not a value") % name), location));
}

void Deg::Compiler::SG::ErrorHelper::TypeDoesNotHaveMembers(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::TypeDoesNotHaveMembers, Diagnostics::ErrorLevel::Error,
			stage, "expression type does not have members to access", location));
}

void Deg::Compiler::SG::ErrorHelper::SetDoesNotHaveQuantity(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SetDoesNotHaveQuantity, Diagnostics::ErrorLevel::Error,
			stage, "set type does not have quantity", location));
}

void Deg::Compiler::SG::ErrorHelper::PredicateSignatureMismatch(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& signature) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::PredicateSignatureMismatch, Diagnostics::ErrorLevel::Error,
			stage, boost::str(boost::format("expected function of type \'%s\'") % signature), location));
}

void Deg::Compiler::SG::ErrorHelper::SetFilterNotOrthogonal(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SetFilterNotOrthogonal, Diagnostics::ErrorLevel::Error,
			stage, "set cannot be expressed as a union of orthogonal ranges", location));
}

void Deg::Compiler::SG::ErrorHelper::SetFilterNotIsolated(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SetFilterNotIsolated, Diagnostics::ErrorLevel::Error,
			stage, "set columns must be isolated", location));
}

void Deg::Compiler::SG::ErrorHelper::SetFilterIrrelevant(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location) {
	report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::SetFilterIrrelevant, Diagnostics::ErrorLevel::Error,
			stage, "set builder clauses must be a relation of a set column", location));
}
