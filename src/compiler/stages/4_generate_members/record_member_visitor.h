#pragma once

#include "compiler/ast/visitor.h"
#include "compiler/sg/node.h"
#include "compiler/sg/module.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateMembers {

class RecordMemberVisitor : public AST::Visitor {
private:
	SG::RecordSymbol& symbol;
	SG::Module& module;

public:
	RecordMemberVisitor(SG::RecordSymbol& symbol, SG::Module& module, Diagnostics::Report& report);

	void VisitRecordMember(AST::RecordMember& member);
};

}
}
}
}
