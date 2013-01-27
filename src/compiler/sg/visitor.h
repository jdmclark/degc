#pragma once

#include "node.h"
#include "compiler/diagnostics/report.h"

#define SGPROTO(x) virtual void Visit##x(SG::x&);
#define SGDEFAULTACTION(x) void Deg::Compiler::SG::Visitor::Visit##x(x& e) { DefaultAction(#x, e); }

#define SGCLASS_SYMBOL(x) \
	x(ProgramSymbol) \
	x(FunctionSymbol) \
	x(RecordSymbol) \
	x(EnumerationSymbol) \
	x(ErrorSymbol)

#define SGCLASS_ALL(x) \
	SGCLASS_SYMBOL(x)

namespace Deg {
namespace Compiler {
namespace SG {

class Visitor {
protected:
	const std::string VisitorName;
	Diagnostics::Report& Report;

	virtual void DefaultAction(const std::string& action, Node& n);

	Visitor(const std::string& name, Diagnostics::Report& report);

public:
	virtual ~Visitor();

	SGCLASS_ALL(SGPROTO);
};

}
}
}
