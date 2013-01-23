#pragma once

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

/* Expressions */

class Expression : public Node {
	ASTVISITOR_ACCEPT_ABSTRACT
};

class NumericLiteralExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	std::string Value;
};

class BooleanLiteralExpression : public Expression {
	ASTVISITOR_ACCEPT
public:
	bool Value;
};

class EmptySetExpression : public Expression {
	ASTVISITOR_ACCEPT
};

class UniversalSetExpression : public Expression {
	ASTVISITOR_ACCEPT
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
	Or
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
public:
	std::string StatementName;
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
};

class EmbedInlineStatement : public Statement {
	ASTVISITOR_ACCEPT
public:
	Statement* Code;
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

/* Program */

class Program : public Declaration {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
	std::string Extends;
	Statement* Code;
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
	std::string Typename;
	std::string Name;
};

class Function : public Declaration {
	ASTVISITOR_ACCEPT
public:
	std::string Name;
	std::vector<FunctionArgument*>* Arguments;
	Expression* Code;
};

/* Records */

class RecordMember : public Node {
	ASTVISITOR_ACCEPT
public:
	std::string Typename;
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
