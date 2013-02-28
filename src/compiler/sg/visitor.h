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
	x(ProgramType) \
	x(RecordType) \
	x(FunctionType) \
	x(EnumerationType) \
	x(ErrorType)

#define SGCLASS_SYMBOL(x) \
	x(NumberSymbol) \
	x(QuantitySymbol) \
	x(BooleanSymbol) \
	x(ProgramSymbol) \
	x(FunctionArgumentSymbol) \
	x(FunctionSymbol) \
	x(RecordMemberSymbol) \
	x(RecordSymbol) \
	x(EnumerationMemberSymbol) \
	x(EnumerationSymbol) \
	x(VariableSymbol) \
	x(ErrorSymbol)

#define SGCLASS_EXPRESSION(x) \
	x(ErrorExpression) \
	x(NumericExpression) \
	x(BooleanExpression) \
	x(TypedSetExpression) \
	x(ConstrainedSetExpression) \
	x(PanicExpression) \
	x(IdentifierExpression) \
	x(FunctionCallExpression) \
	x(MemberAccessExpression) \
	x(UnaryExpression) \
	x(ExistsExpression) \
	x(InfixExpression) \
	x(FunctionIfElseExpression) \
	x(SetClauseExpression)

#define SGCLASS_STATEMENT(x) \
	x(CompoundStatement) \
	x(AssertStatement) \
	x(EmbedStatement) \
	x(DisjunctionStatement) \
	x(ForAllStatement) \
	x(ForAnyStatement) \
	x(ForBestStatement) \
	x(IfStatement) \
	x(IfElseStatement) \
	x(TakeStatement) \
	x(LimitStatement) \
	x(ErrorStatement)

#define SGCLASS_ALL(x) \
	SGCLASS_TYPE(x) \
	SGCLASS_SYMBOL(x) \
	SGCLASS_EXPRESSION(x) \
	SGCLASS_STATEMENT(x)

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
