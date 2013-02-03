#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "compiler/ast/node.h"

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
};

class Symbol : public Node {
	SGVISITOR_ACCEPT_ABSTRACT
};

class RecordSymbol;
class EnumerationSymbol;
class ProgramSymbol;

// Types

class NumberType : public Type {
	SGVISITOR_ACCEPT
};

class BooleanType : public Type {
	SGVISITOR_ACCEPT
};

class SetType : public Type {
	SGVISITOR_ACCEPT
};

class ConstrainedSetType : public Type {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;

	ConstrainedSetType(RecordSymbol* ElementType);
};

class ProgramType : public Type {
	SGVISITOR_ACCEPT
public:
	ProgramSymbol* ElementType;

	ProgramType(ProgramSymbol* ElementType);
};

class RecordType : public Type {
	SGVISITOR_ACCEPT
public:
	RecordSymbol* ElementType;

	RecordType(RecordSymbol* ElementType);
};

class FunctionType : public Type {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> ReturnType;
	std::vector<std::unique_ptr<Type>> ArgumentTypes;
};

class EnumerationType : public Type {
	SGVISITOR_ACCEPT
public:
	EnumerationSymbol* ElementType;

	EnumerationType(EnumerationSymbol* ElementType);
};

class ErrorType : public Type {
	SGVISITOR_ACCEPT
};

// Symbols

class ParentSymbol : public Symbol {
	SGVISITOR_ACCEPT_ABSTRACT
private:
	std::vector<std::unique_ptr<Symbol>> children;
	std::unordered_map<std::string, Symbol*> children_map;

public:
	inline bool IsMember(const std::string& name) const {
		return children_map.count(name) > 0;
	}

	template <typename T, typename... Args> void MakeMember(const std::string& name, Args&&... args) {
		children.push_back(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
		children_map.insert(std::make_pair(name, children.back().get()));
	}

	Symbol& GetMember(const std::string& name) {
		auto it = children_map.find(name);
		if(it == children_map.end()) {
			throw std::exception();
		}

		return *it->second;
	}

	std::vector<std::unique_ptr<Symbol>>::iterator begin() {
		return children.begin();
	}

	std::vector<std::unique_ptr<Symbol>>::iterator end() {
		return children.end();
	}

	size_t children_size() const {
		return children.size();
	}
};

class NumberSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class QuantitySymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class BooleanSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class ProgramSymbol : public ParentSymbol {
	SGVISITOR_ACCEPT
public:
	AST::Program* ast_program;
	Symbol* Base;

	ProgramSymbol();
};

class RecordMemberSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> InputType;
	unsigned int Offset;

	RecordMemberSymbol(std::unique_ptr<Type>& InputType);
};

class RecordSymbol : public ParentSymbol {
	SGVISITOR_ACCEPT
public:
	AST::Record* ast_record;
	Symbol* QuantityMember;

	RecordSymbol();
};

class FunctionArgumentSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::unique_ptr<Type> InputType;
	unsigned int Offset;

	FunctionArgumentSymbol(std::unique_ptr<Type>& InputType);
};

class FunctionSymbol : public ParentSymbol {
	SGVISITOR_ACCEPT
public:
	AST::Function* ast_function;
	std::unique_ptr<Type> CodomainType;
};

class EnumerationMemberSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	unsigned int Value;

	EnumerationMemberSymbol(unsigned int Value);
};

class EnumerationSymbol : public ParentSymbol {
	SGVISITOR_ACCEPT
public:
	AST::Enumeration* ast_enumeration;
};

class ErrorSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

}
}
}
