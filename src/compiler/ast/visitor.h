#pragma once

#include "node.h"
#include "compiler/diagnostics/report.h"

#define AVPROTO(x) virtual void Visit##x(AST::x&);
#define AVCONSUME(x) virtual void Visit##x(AST::x&);
#define AVDEFAULTACTION(x) void Deg::Compiler::AST::Visitor::Visit##x(x& e) { DefaultAction(#x, e); }

#define ASTCLASS_TRANSLATION_UNIT(x) \
	x(TranslationUnit)

#define ASTCLASS_ALL(x) \
	ASTCLASS_TRANSLATION_UNIT(x)

namespace Deg {
namespace Compiler {
namespace AST {

class Visitor {
protected:
	const std::string VisitorName;
	Diagnostics::Report& Report;

	virtual void DefaultAction(const std::string& action, Node& n);

	Visitor(const std::string& name, Diagnostics::Report& report);

public:
	virtual ~Visitor();

	ASTCLASS_ALL(AVPROTO);
};

}
}
}
