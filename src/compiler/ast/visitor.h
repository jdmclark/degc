#pragma once

#include "node.h"
#include "compiler/diagnostics/report.h"

#define AVPROTO(x) virtual void Visit##x(AST::x&);
#define AVCONSUME(x) virtual void Visit##x(AST::x&) { }
#define AVDEFAULTACTION(x) void Deg::Compiler::AST::Visitor::Visit##x(x& e) { DefaultAction(#x, e); }

#define ASTCLASS_TYPENAME(x) \
	x(NamedTypename) \
	x(SetTypename) \
	x(FunctionTypename)

#define ASTCLASS_EXPRESSION(x) \
	x(NumericLiteralExpression) \
	x(BooleanLiteralExpression) \
	x(TypedSetExpression) \
	x(ConstrainedSetExpression) \
	x(PanicExpression) \
	x(IdentifierExpression) \
	x(FunctionCallExpression) \
	x(MemberAccessExpression) \
	x(UnaryExpression) \
	x(InfixExpression) \
	x(FunctionIfElseExpression)

#define ASTCLASS_STATEMENT(x) \
	x(CompoundStatement) \
	x(AssertStatement) \
	x(EmbedStatement) \
	x(EmbedInlineStatement) \
	x(DisjunctionStatement) \
	x(ForAllStatement) \
	x(ForAnyStatement) \
	x(ForBestStatement) \
	x(IfStatement) \
	x(IfElseStatement) \
	x(TakeStatement) \
	x(LimitStatement)

#define ASTCLASS_DECLARATION(x) \
	x(Program) \
	x(FunctionArgument) \
	x(Function)\
	x(RecordMember) \
	x(Record) \
	x(EnumerationMember) \
	x(Enumeration)

#define ASTCLASS_TRANSLATION_UNIT(x) \
	x(ModuleName) \
	x(SymbolImport) \
	x(ModuleImport) \
	x(TranslationUnit)

#define ASTCLASS_ALL(x) \
	ASTCLASS_TYPENAME(x) \
	ASTCLASS_EXPRESSION(x) \
	ASTCLASS_STATEMENT(x) \
	ASTCLASS_DECLARATION(x) \
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
