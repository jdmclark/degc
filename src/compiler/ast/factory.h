#pragma once

#include "node.h"
#include <memory>

#define MAKE_NOLINES(x)							\
	x* val = new x;								\
	nodes.push_back(std::unique_ptr<x>(val));

#define MAKE(x)									\
	MAKE_NOLINES(x)								\
	val->Location = yyl;

namespace Deg {
namespace Compiler {
namespace AST {

class Factory {
private:
	std::vector<std::unique_ptr<Node>> nodes;

	template <typename T> class ValueStorageNode : public Node {
	public:
		ValueStorageNode() {
			return;
		}

		T Value;

		void Accept(Visitor& v) {
			return;
		}
	};

public:
	template <typename T> T* StoreValue(const T& value) {
		MAKE_NOLINES(ValueStorageNode<T>);
		val->Value = value;
		return &val->Value;
	}

	template <typename T> std::vector<T*>* MakeList(T* initial_value) {
		MAKE_NOLINES(ValueStorageNode<std::vector<T*>>);
		val->Value.push_back(initial_value);
		return &val->Value;
	}

	template <typename T> std::vector<T*>* MakeList() {
		MAKE_NOLINES(ValueStorageNode<std::vector<T*>>);
		return &val->Value;
	}

	NamedTypename* MakeNamedTypename(const std::string& Value, const Diagnostics::ErrorLocation& yyl);
	SetTypename* MakeSetTypename(const std::string& RecordType, const Diagnostics::ErrorLocation& yyl);
	FunctionTypename* MakeFunctionTypename(std::vector<Typename*>* DomainType, Typename* CodomainType, const Diagnostics::ErrorLocation& yyl);

	NumericLiteralExpression* MakeNumericLiteralExpression(const std::string& Value, const Diagnostics::ErrorLocation& yyl);
	BooleanLiteralExpression* MakeBooleanLiteralExpression(bool Value, const Diagnostics::ErrorLocation& yyl);
	TypedSetExpression* MakeTypedSetExpression(const std::string& Typename, const Diagnostics::ErrorLocation& yyl);
	ConstrainedSetExpression* MakeConstrainedSetExpression(const std::string& Typename, Expression* Constraint, const Diagnostics::ErrorLocation& yyl);
	PanicExpression* MakePanicExpression(const Diagnostics::ErrorLocation& yyl);
	IdentifierExpression* MakeIdentifierExpression(const std::string& Identifier, const Diagnostics::ErrorLocation& yyl);
	FunctionCallExpression* MakeFunctionCallExpression(Expression* Target, std::vector<Expression*>* Arguments, const Diagnostics::ErrorLocation& yyl);
	MemberAccessExpression* MakeMemberAccessExpression(Expression* Target, const std::string& MemberName, const Diagnostics::ErrorLocation& yyl);
	UnaryExpression* MakeUnaryExpression(UnaryOperator Operator, Expression* Value, const Diagnostics::ErrorLocation& yyl);
	ExistsExpression* MakeExistsExpression(Expression* Value, const Diagnostics::ErrorLocation& yyl);
	InfixExpression* MakeInfixExpression(InfixOperator Operator, Expression* LeftValue, Expression* RightValue, const Diagnostics::ErrorLocation& yyl);
	FunctionIfElseExpression* MakeFunctionIfElseExpression(Expression* Predicate, Expression* Code, Expression* ElseCode, const Diagnostics::ErrorLocation& yyl);

	NamedStatement* MakeNamedStatement(const std::string& Name, Statement* Code, const Diagnostics::ErrorLocation& yyl);
	CompoundStatement* MakeCompoundStatement(std::vector<Statement*>* Statements, const Diagnostics::ErrorLocation& yyl);
	AssertStatement* MakeAssertStatement(Expression* Value, const Diagnostics::ErrorLocation& yyl);
	EmbedStatement* MakeEmbedStatement(Expression* Value, std::vector<Expression*>* Arguments, const Diagnostics::ErrorLocation& yyl);
	EmbedInlineStatement* MakeEmbedInlineStatement(Statement* Code, const Diagnostics::ErrorLocation& yyl);
	DisjunctionStatement* MakeDisjunctionStatement(std::vector<Statement*>* Statements, const Diagnostics::ErrorLocation& yyl);
	ForAllStatement* MakeForAllStatement(const std::string& Typename, const std::string& ElementName, Expression* Set, Statement* Code, const Diagnostics::ErrorLocation& yyl);
	ForAnyStatement* MakeForAnyStatement(const std::string& Typename, const std::string& ElementName, Expression* Set, Statement* Code, const Diagnostics::ErrorLocation& yyl);
	ForBestStatement* MakeForBestStatement(const std::string& Typename, const std::string& ElementName, Expression* Predicate, Expression* Set, Statement* Code, const Diagnostics::ErrorLocation& yyl);
	IfStatement* MakeIfStatement(Expression* Predicate, Statement* Code, const Diagnostics::ErrorLocation& yyl);
	IfElseStatement* MakeIfElseStatement(Expression* Predicate, Statement* Code, Statement* ElseCode, const Diagnostics::ErrorLocation& yyl);
	TakeStatement* MakeTakeStatement(Expression* Amount, Expression* Set, const Diagnostics::ErrorLocation& yyl);
	LimitStatement* MakeLimitStatement(Expression* Amount, Expression* Set, const Diagnostics::ErrorLocation& yyl);

	Program* MakeProgram(const std::string& Name, std::vector<FunctionArgument*>* Arguments, const std::string& Extends, std::vector<Expression*>* BaseArguments, Statement* Code, const Diagnostics::ErrorLocation& yyl);

	FunctionArgument* MakeFunctionArgument(Typename* Type, const std::string& Name, const Diagnostics::ErrorLocation& yyl);
	Function* MakeFunction(const std::string& Name, std::vector<FunctionArgument*>* Arguments, Typename* Codomain, Expression* Code, const Diagnostics::ErrorLocation& yyl);

	RecordMember* MakeRecordMember(Typename* Type, const std::string& Name, const Diagnostics::ErrorLocation& yyl);
	Record* MakeRecord(const std::string& Name, std::vector<RecordMember*>* Members, const Diagnostics::ErrorLocation& yyl);

	EnumerationMember* MakeEnumerationMember(const std::string& Name, const Diagnostics::ErrorLocation& yyl);
	Enumeration* MakeEnumeration(const std::string& Name, std::vector<EnumerationMember*>* Members, const Diagnostics::ErrorLocation& yyl);

	ModuleName* MakeModuleName(const std::string& Name, const Diagnostics::ErrorLocation& yyl);
	SymbolImport* MakeSymbolImport(const std::string& OriginalName, const std::string& ImportName, const Diagnostics::ErrorLocation& yyl);
	ModuleImport* MakeModuleImport(ModuleName* Name, std::vector<SymbolImport*>* Symbols, const Diagnostics::ErrorLocation& yyl);
	TranslationUnit* MakeTranslationUnit(ModuleName* Name, std::vector<ModuleImport*>* Imports, std::vector<Declaration*>* Declarations, const Diagnostics::ErrorLocation& yyl);
};

}
}
}
