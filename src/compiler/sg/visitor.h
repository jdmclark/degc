#pragma once

#include "node.h"
#include "compiler/diagnostics/report.h"

#define SGPROTO(x) virtual void Visit##x(SG::x&);
#define SGCONSUME(x) void Visit##x(SG::x&) { };
#define SGDEFAULTACTION(x) void Deg::Compiler::SG::Visitor::Visit##x(x& e) { DefaultAction(#x, e); }

#define SGCLASS_TYPE(x) \
	x(NumberType) \
	x(BooleanType) \
	x(SetType) \
	x(ConstrainedSetType) \
	x(ProgramType) \
	x(RecordType) \
	x(FunctionType) \
	x(EnumerationType) \
	x(ErrorType)

#define SGCLASS_SYMBOL(x) \
	x(NumberSymbol) \
	x(QuantitySymbol) \
	x(BooleanSymbol) \
	x(SetSymbol) \
	x(ProgramSymbol) \
	x(FunctionArgumentSymbol) \
	x(FunctionSymbol) \
	x(RecordMemberSymbol) \
	x(RecordSymbol) \
	x(EnumerationMemberSymbol) \
	x(EnumerationSymbol) \
	x(ErrorSymbol)

#define SGCLASS_ALL(x) \
	SGCLASS_TYPE(x) \
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
