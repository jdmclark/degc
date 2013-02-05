#pragma once

#include "compiler/ast/visitor.h"
#include "compiler/sg/scope_stack.h"

namespace Deg {
namespace Compiler {
namespace Stages {
namespace GenerateExpressions {

class StatementVisitor : public AST::Visitor {
private:
	SG::ScopeStack& scope;
	const bool InsideLoop;

public:
	StatementVisitor(SG::ScopeStack& scope, bool InsideLoop, Diagnostics::Report& report);

	std::string GeneratedStatementName;
	std::unique_ptr<SG::Statement> GeneratedStatement;

	void VisitNamedStatement(AST::NamedStatement& n);
	void VisitCompoundStatement(AST::CompoundStatement& n);
	void VisitAssertStatement(AST::AssertStatement& n);
	void VisitEmbedStatement(AST::EmbedStatement& n);
	void VisitEmbedInlineStatement(AST::EmbedInlineStatement& n);
	void VisitDisjunctionStatement(AST::DisjunctionStatement& n);
	void VisitForAllStatement(AST::ForAllStatement& n);
	void VisitForAnyStatement(AST::ForAnyStatement& n);
	void VisitForBestStatement(AST::ForBestStatement& n);
	void VisitIfStatement(AST::IfStatement& n);
	void VisitIfElseStatement(AST::IfElseStatement& n);
	void VisitTakeStatement(AST::TakeStatement& n);
	void VisitLimitStatement(AST::LimitStatement& n);
};

}
}
}
}
