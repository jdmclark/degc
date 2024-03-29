#pragma once

#include "runtime/math/fixed.h"
#include "compiler/diagnostics/errorlocation.h"
#include <string>
#include <vector>

#define ASTVISITOR_ACCEPT_ABSTRACT								\
	public:														\
	virtual void Accept(Deg::Compiler::AST::Visitor& v) = 0;	\
	private:

#define ASTVISITOR_ACCEPT										\
	public:														\
	virtual void Accept(Deg::Compiler::AST::Visitor& v);		\
	private:

namespace Deg {
namespace Compiler {
namespace AST {

class Visitor;

class Node {
	ASTVISITOR_ACCEPT_ABSTRACT
protected:
	Node();

public:
	virtual ~Node();

	Diagnostics::ErrorLocation Location;
};

/* Global nodes */

class Declaration : public Node {
	ASTVISITOR_ACCEPT_ABSTRACT
};

class Typename : public Node {
	ASTVISITOR_ACCEPT_ABSTRACT
};

class NamedTypename : public Typename {
	ASTVISITOR_ACCEPT
public:
	std::string Value;
};

class SetTypename : public Typename {
	ASTVISITOR_ACCEPT
public:
	std::string RecordType;
};

class FunctionTypename : public Typename {
	ASTVISITOR_ACCEPT
public:
	std::vector<Typename*>* DomainType;
	Typename* CodomainType;
};

/* Expressions */

class Expression : public Node {
	ASTVISITOR_ACCEPT_ABSTRACT
};

class NumericLiteralExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	Runtime::Math::DefaultFixed Value;
};

class BooleanLiteralExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	bool Value;
};

class TypedSetExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	std::string Typename;
};

class ConstrainedSetExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	std::string Typename;
	std::string ElementName;
	Expression* Constraint;
};

class PanicExpression : public Expression {
	ASTVISITOR_ACCEPT
};

class IdentifierExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	std::string Identifier;
};

class FunctionCallExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	Expression* Target;
	std::vector<Expression*>* Arguments;
};

class MemberAccessExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	Expression* Target;
	std::string MemberName;
};

enum class UnaryOperator {
	Minus,
	Not
};

class UnaryExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	UnaryOperator Operator;
	Expression* Value;
};

class ExistsExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	Expression* Value;
};

enum class InfixOperator {
	Addition,
	Subtraction,
	Multiplication,
	Division,

	Greater,
	GreaterEqual,
	Less,
	LessEqual,
	Equal,
	NotEqual,

	And,
	Or,

	Union,
	Intersect,
	SetMinus
};

class InfixExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	InfixOperator Operator;
	Expression* LeftValue;
	Expression* RightValue;
};

/* Program Statements */

class Statement : public Node {
	ASTVISITOR_ACCEPT_ABSTRACT
};

class NamedStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
	Statement* Code;
};

class CompoundStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	std::vector<Statement*>* Statements;

	inline std::vector<Statement*>::iterator begin() {
		return Statements->begin();
	}

	inline std::vector<Statement*>::iterator begin() const {
		return Statements->begin();
	}

	inline std::vector<Statement*>::iterator end() {
		return Statements->end();
	}

	inline std::vector<Statement*>::iterator end() const {
		return Statements->end();
	}
};

class AssertStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Expression* Value;
};

class EmbedStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Expression* Value;
	std::vector<Expression*>* Arguments;
};

class EmbedInlineStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Statement* Code;
};

class DisjunctionStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	std::vector<Statement*>* Statements;

	inline std::vector<Statement*>::iterator begin() {
		return Statements->begin();
	}

	inline std::vector<Statement*>::iterator begin() const {
		return Statements->begin();
	}

	inline std::vector<Statement*>::iterator end() {
		return Statements->end();
	}

	inline std::vector<Statement*>::iterator end() const {
		return Statements->end();
	}
};

class ForAllStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	std::string Typename;
	std::string ElementName;
	Expression* Set;
	Statement* Code;
};

class ForAnyStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	std::string Typename;
	std::string ElementName;
	Expression* Set;
	Statement* Code;
};

class ForBestStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	std::string Typename;
	std::string ElementName;
	Expression* Predicate;
	Expression* Set;
	Statement* Code;
};

class IfStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Expression* Predicate;
	Statement* Code;
};

class IfElseStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Expression* Predicate;
	Statement* Code;
	Statement* ElseCode;
};

class TakeStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Expression* Amount;
	Expression* Set;
};

class LimitStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Expression* Amount;
	Expression* Set;
};

/* Functions */

class FunctionIfElseExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	Expression* Predicate;
	Expression* Code;
	Expression* ElseCode;
};

class FunctionArgument : public Expression {
	ASTVISITOR_ACCEPT
public:
	Typename* Type;
	std::string Name;
};

class Function : public Declaration {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
	std::vector<FunctionArgument*>* Arguments;
	Typename* Codomain;
	Expression* Code;
};

/* Program */

class Program : public Declaration {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
	std::vector<FunctionArgument*>* Arguments;
	std::string Extends;
	std::vector<Expression*>* BaseArguments;
	Statement* Code;
};

/* Records */

class RecordMember : public Node {
	ASTVISITOR_ACCEPT
public:
	Typename* Type;
	std::string Name;
};

class Record : public Declaration {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
	std::vector<RecordMember*>* Members;
};

/* Enumeration */

class EnumerationMember : public Node {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
};

class Enumeration : public Declaration {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
	std::vector<EnumerationMember*>* Members;
};

/* Translation unit */

class ModuleName : public Node {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
};

class SymbolImport : public Node {
	ASTVISITOR_ACCEPT
public:
	std::string OriginalName;
	std::string ImportName;
};

class ModuleImport : public Node {
	ASTVISITOR_ACCEPT
public:
	ModuleName* Name;
	std::vector<SymbolImport*>* Symbols;
};

class TranslationUnit : public Node {
	ASTVISITOR_ACCEPT
public:
	ModuleName* Name;
	std::vector<ModuleImport*>* Imports;
	std::vector<Declaration*>* Declarations;
};

}
}
}
