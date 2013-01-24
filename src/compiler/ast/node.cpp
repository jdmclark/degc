#include "node.h"
#include "visitor.h"

Deg::Compiler::AST::Node::Node() {
	return;
}

Deg::Compiler::AST::Node::~Node() {
	return;
}

#define ASTVISITOR_ACCEPT_IMPL(x)						\
	void Deg::Compiler::AST::x::Accept(Visitor& v) {	\
		v.Visit##x(*this);								\
		return;											\
	}

ASTVISITOR_ACCEPT_IMPL(NumericLiteralExpression);
ASTVISITOR_ACCEPT_IMPL(BooleanLiteralExpression);
ASTVISITOR_ACCEPT_IMPL(EmptySetExpression);
ASTVISITOR_ACCEPT_IMPL(UniversalSetExpression);
ASTVISITOR_ACCEPT_IMPL(TypedSetExpression);
ASTVISITOR_ACCEPT_IMPL(ConstrainedSetExpression);
ASTVISITOR_ACCEPT_IMPL(PanicExpression);
ASTVISITOR_ACCEPT_IMPL(IdentifierExpression);
ASTVISITOR_ACCEPT_IMPL(FunctionCallExpression);
ASTVISITOR_ACCEPT_IMPL(MemberAccessExpression);
ASTVISITOR_ACCEPT_IMPL(UnaryExpression);
ASTVISITOR_ACCEPT_IMPL(InfixExpression);

ASTVISITOR_ACCEPT_IMPL(CompoundStatement);
ASTVISITOR_ACCEPT_IMPL(AssertStatement);
ASTVISITOR_ACCEPT_IMPL(EmbedStatement);
ASTVISITOR_ACCEPT_IMPL(EmbedInlineStatement);
ASTVISITOR_ACCEPT_IMPL(DisjunctionStatement);
ASTVISITOR_ACCEPT_IMPL(ForAllStatement);
ASTVISITOR_ACCEPT_IMPL(ForAnyStatement);
ASTVISITOR_ACCEPT_IMPL(ForBestStatement);
ASTVISITOR_ACCEPT_IMPL(IfStatement);
ASTVISITOR_ACCEPT_IMPL(IfElseStatement);
ASTVISITOR_ACCEPT_IMPL(TakeStatement);
ASTVISITOR_ACCEPT_IMPL(LimitStatement);

ASTVISITOR_ACCEPT_IMPL(Program);

ASTVISITOR_ACCEPT_IMPL(FunctionIfElseExpression);
ASTVISITOR_ACCEPT_IMPL(FunctionArgument);
ASTVISITOR_ACCEPT_IMPL(Function);

ASTVISITOR_ACCEPT_IMPL(RecordMember);
ASTVISITOR_ACCEPT_IMPL(Record);

ASTVISITOR_ACCEPT_IMPL(EnumerationMember);
ASTVISITOR_ACCEPT_IMPL(Enumeration);

ASTVISITOR_ACCEPT_IMPL(ModuleName);
ASTVISITOR_ACCEPT_IMPL(SymbolImport);
ASTVISITOR_ACCEPT_IMPL(ModuleImport);
ASTVISITOR_ACCEPT_IMPL(TranslationUnit);
