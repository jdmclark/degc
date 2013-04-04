#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "runtime/math/fixed.h"
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

class Statement : public Node {
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
	std::string UniversalUniqueName;
	AST::Program* ast_program;
	ProgramSymbol* Base;
	std::unique_ptr<Statement> Statements;

	ProgramSymbol();
	bool InheritsFrom(const ProgramSymbol& program) const;
};

class RecordMemberSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> InputType;
	unsigned int Index;

	RecordMemberSymbol(std::unique_ptr<Type>& InputType, unsigned int Index);
};

class RecordSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::string UniversalUniqueName;
	AST::Record* ast_record;
	Node* QuantityMember;
	Scope Members;

	RecordSymbol();
};

class FunctionArgumentSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> InputType;
	unsigned int Index;

	FunctionArgumentSymbol(std::unique_ptr<Type>& InputType, unsigned int Index);
};

class FunctionSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::string UniversalUniqueName;
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

class VariableSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;
	unsigned int Offset;

	VariableSymbol(RecordSymbol* ElementType);
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
	Deg::Runtime::Math::DefaultFixed Value;

	NumericExpression(Deg::Runtime::Math::DefaultFixed Value);
};

class BooleanExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	bool Value;

	BooleanExpression(bool Value);
};

class TypedSetExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;
	Diagnostics::ErrorLocation Location;

	TypedSetExpression(RecordSymbol* ElementType, const Diagnostics::ErrorLocation& Location);
};

class ConstrainedSetExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;
	std::unique_ptr<Expression> Filter;
	Diagnostics::ErrorLocation Location;

	ConstrainedSetExpression(RecordSymbol* ElementType, std::unique_ptr<Expression>& Filter, const Diagnostics::ErrorLocation& Location);
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

class MemberAccessExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Base;
	RecordMemberSymbol* Member;

	MemberAccessExpression(std::unique_ptr<Expression>& Base, RecordMemberSymbol* Member);
};

class UnaryExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Value;
	AST::UnaryOperator Operator;

	UnaryExpression(std::unique_ptr<Expression>& Value, AST::UnaryOperator Operator);
};

class ExistsExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Value;
	RecordSymbol* ElementType;

	ExistsExpression(std::unique_ptr<Expression>& Value, RecordSymbol* ElementType);
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
	std::unique_ptr<Expression> Predicate;
	std::unique_ptr<Expression> IfCode, ElseCode;

	FunctionIfElseExpression(std::unique_ptr<Expression>& Predicate, std::unique_ptr<Expression>& IfCode, std::unique_ptr<Expression>& ElseCode);
};

class SetClauseExpression : public Expression {
	SGVISITOR_ACCEPT
public:
	RecordMemberSymbol* Member;
	AST::InfixOperator Operator;
	std::unique_ptr<Expression> Value;

	SetClauseExpression(RecordMemberSymbol* Member, AST::InfixOperator Operator, std::unique_ptr<Expression>& Value);
};

// Statements

class CompoundStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	Scope Statements;
};

class AssertStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Value;

	AssertStatement(std::unique_ptr<Expression>& Value);
};

class EmbedStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Value;

	EmbedStatement(std::unique_ptr<Expression>& Value);
};

class DisjunctionStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	std::vector<std::unique_ptr<Statement>> Statements;

	inline std::vector<std::unique_ptr<Statement>>::iterator begin() {
		return Statements.begin();
	}

	inline std::vector<std::unique_ptr<Statement>>::const_iterator begin() const {
		return Statements.begin();
	}

	inline std::vector<std::unique_ptr<Statement>>::iterator end() {
		return Statements.end();
	}

	inline std::vector<std::unique_ptr<Statement>>::const_iterator end() const {
		return Statements.end();
	}
};

class ForAllStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	Scope Iterators;
	std::unique_ptr<Expression> Set;
	std::unique_ptr<Statement> Code;
};

class ForAnyStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	Scope Iterators;
	std::unique_ptr<Expression> Set;
	std::unique_ptr<Statement> Code;
};

class ForBestStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	Scope Iterators;
	std::unique_ptr<Expression> Predicate;
	std::unique_ptr<Expression> Set;
	std::unique_ptr<Statement> Code;
};

class IfStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Predicate;
	std::unique_ptr<Statement> Code;

	IfStatement(std::unique_ptr<Expression>& Predicate, std::unique_ptr<Statement>& Code);
};

class IfElseStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Expression> Predicate;
	std::unique_ptr<Statement> Code;
	std::unique_ptr<Statement> ElseCode;

	IfElseStatement(std::unique_ptr<Expression>& Predicate, std::unique_ptr<Statement>& Code, std::unique_ptr<Statement>& ElseCode);
};

class TakeStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	AST::TakeStatement* ast_node;
	std::unique_ptr<Expression> Amount;
	std::unique_ptr<Expression> Set;

	TakeStatement(AST::TakeStatement* ast_node, std::unique_ptr<Expression>& Amount, std::unique_ptr<Expression>& Set);
};

class LimitStatement : public Statement {
	SGVISITOR_ACCEPT
public:
	AST::LimitStatement* ast_node;
	std::unique_ptr<Expression> Amount;
	std::unique_ptr<Expression> Set;

	LimitStatement(AST::LimitStatement* ast_node, std::unique_ptr<Expression>& Amount, std::unique_ptr<Expression>& Set);
};

class ErrorStatement : public Statement {
	SGVISITOR_ACCEPT
};

}
}
}
