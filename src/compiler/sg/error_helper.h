#pragma once

#include <string>
#include "compiler/diagnostics/report.h"

namespace Deg {
namespace Compiler {
namespace SG {
namespace ErrorHelper {

void SymbolRedefinition(Diagnostics::Report& report, const std::string& stage, const Diagnostics::ErrorLocation& location, const std::string& symbol);

}
}
}
}
