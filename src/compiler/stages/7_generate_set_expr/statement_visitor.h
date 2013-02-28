#pragma once

#include "compiler/sg/visitor.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateSetExpressions {

class StatementVisitor : public SG::Visitor {
public:
	StatementVisitor(Diagnostics::Report& report);

	SGCONSUME(ErrorStatement);
	SGCONSUME(EmbedStatement);

	void VisitCompoundStatement(SG::CompoundStatement& n);
	void VisitAssertStatement(SG::AssertStatement& n);
	void VisitForAllStatement(SG::ForAllStatement& n);
	void VisitDisjunctionStatement(SG::DisjunctionStatement& n);
	void VisitForAnyStatement(SG::ForAnyStatement& n);
	void VisitForBestStatement(SG::ForBestStatement& n);
	void VisitIfStatement(SG::IfStatement& n);
	void VisitIfElseStatement(SG::IfElseStatement& n);
	void VisitTakeStatement(SG::TakeStatement& n);
	void VisitLimitStatement(SG::LimitStatement& n);
};

}
}
}
}
