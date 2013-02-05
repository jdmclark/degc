#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "compiler/ast/node.h"
#include "scope.h"

#define SGVISITOR_ACCEPT_ABSTRACT								\
	public:														\
	virtual void Accept(Deg::Compiler::SG::Visitor& v) = 0;		\
	private:

#define SGVISITOR_ACCEPT										\
	public:														\
	virtual void Accept(Deg::Compiler::SG::Visitor& v);			\
	private:

namespace Deg {
namespace Compiler {
namespace SG {

class Visitor;

class Node {
	SGVISITOR_ACCEPT_ABSTRACT
public:
	virtual ~Node();
};

class Type : public Node {
	SGVISITOR_ACCEPT_ABSTRACT

public:
	virtual ~Type();

	virtual std::unique_ptr<Type> Clone() const = 0;
	virtual bool CanAcceptValueOfType(const Type&) const = 0;
};

class Symbol : public Node {
	SGVISITOR_ACCEPT_ABSTRACT
};

class Expression : public Node {
	SGVISITOR_ACCEPT_ABSTRACT
};

class RecordSymbol;
class EnumerationSymbol;
class ProgramSymbol;

// Types

class NumberType : public Type {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

class BooleanType : public Type {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

class SetType : public Type {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;

	SetType(RecordSymbol* ElementType);
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

class ProgramType : public Type {
	SGVISITOR_ACCEPT
public:
	ProgramSymbol* ElementType;

	ProgramType(ProgramSymbol* ElementType);
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

class RecordType : public Type {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;

	RecordType(RecordSymbol* ElementType);
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

class FunctionType : public Type {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> ReturnType;
	std::vector<std::unique_ptr<Type>> ArgumentTypes;
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

class EnumerationType : public Type {
	SGVISITOR_ACCEPT
public:
	EnumerationSymbol* ElementType;

	EnumerationType(EnumerationSymbol* ElementType);
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

class ErrorType : public Type {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> Clone() const;
	bool CanAcceptValueOfType(const Type&) const;
};

// Symbols

class NumberSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class QuantitySymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class BooleanSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class ProgramSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Program* ast_program;
	ProgramSymbol* Base;
	Scope Statements;

	ProgramSymbol();
	bool InheritsFrom(const ProgramSymbol& program) const;
};

class RecordMemberSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> InputType;
	unsigned int Offset;

	RecordMemberSymbol(std::unique_ptr<Type>& InputType);
};

class RecordSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Record* ast_record;
	Node* QuantityMember;
	Scope Members;

	RecordSymbol();
};

class FunctionArgumentSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> InputType;
	unsigned int Offset;

	FunctionArgumentSymbol(std::unique_ptr<Type>& InputType);
};

class FunctionSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Function* ast_function;
	std::unique_ptr<Type> CodomainType;
	Scope Arguments;
	std::unique_ptr<Expression> Code;
};

class EnumerationMemberSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	unsigned int Value;

	EnumerationMemberSymbol(unsigned int Value);
};

class EnumerationSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Enumeration* ast_enumeration;
	Scope Values;
};

class ErrorSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

// Expressions

class ErrorExpression : public Expression {
	SGVISITOR_ACCEPT
};

class NumericExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	Runtime::Math::DefaultFixed Value;

	NumericExpression(Runtime::Math::DefaultFixed Value);
};

class BooleanExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	bool Value;

	BooleanExpression(bool Value);
};

class EmptySetExpression : public Expression {
	SGVISITOR_ACCEPT
};

class UniversalSetExpression : public Expression {
	SGVISITOR_ACCEPT
};

class TypedSetExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;

	TypedSetExpression(RecordSymbol* ElementType);
};

class ConstrainedSetExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;
	std::unique_ptr<Expression> Filter;

	ConstrainedSetExpression(RecordSymbol* ElementType, std::unique_ptr<Expression>& Filter);
};

class PanicExpression : public Expression {
	SGVISITOR_ACCEPT
};

class IdentifierExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	Node* ReferencedNode;

	IdentifierExpression(Node* ReferencedNode);
};

class FunctionCallExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> FunctionTargetExpression;
	std::vector<std::unique_ptr<Expression>> ArgumentExpressions;
};

class UnaryExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Value;
	AST::UnaryOperator Operator;

	UnaryExpression(std::unique_ptr<Expression>& Value, AST::UnaryOperator Operator);
};

class InfixExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> LeftValue, RightValue;
	AST::InfixOperator Operator;

	InfixExpression(std::unique_ptr<Expression>& LeftValue, std::unique_ptr<Expression>& RightValue, AST::InfixOperator Operator);
};

class FunctionIfElseExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> IfCode, ElseCode;

	FunctionIfElseExpression(std::unique_ptr<Expression>& IfCode, std::unique_ptr<Expression>& ElseCode);
};

}
}
}
