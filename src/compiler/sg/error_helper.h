#pragma once

#include <string>
#include "compiler/diagnostics/report.h"

namespace Deg {
namespace Compiler {
namespace SG {
namespace ErrorHelper {

void SymbolRedefinition(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& symbol);
void UndefinedTypename(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name);
void UndefinedSymbol(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name);
void RecordNameExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name);
void MeaninglessQuantity(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void BooleanExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void NumberExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void SetExpected(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void SetDomainMismatch(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void SymbolNotExpression(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& name);
void TypeDoesNotHaveMembers(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void SetDoesNotHaveQuantity(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void PredicateSignatureMismatch(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& signature);
void SetFilterNotOrthogonal(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void SetFilterNotIsolated(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);
void SetFilterIrrelevant(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location);

}
}
}
}
