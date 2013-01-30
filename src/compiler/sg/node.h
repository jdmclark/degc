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
};

class ProgramType : public Type {
	SGVISITOR_ACCEPT
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

template <typename T> class ParentSymbol : public Symbol {
	SGVISITOR_ACCEPT_ABSTRACT
private:
	std::vector<std::unique_ptr<T>> children;
	std::unordered_map<std::string, T*> children_map;
public:
	bool IsMember(const std::string& name) const {
		return children_map.count(name) > 0;
	}

	template <typename... Args> void MakeMember(const std::string& name, Args&&... args) {
		children.push_back(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
		children_map.insert(std::make_pair(name, children.back().get()));
	}

	T& GetMember(const std::string& name) {
		auto it = children_map.find(name);
		if(it == children_map.end()) {
			throw std::exception();
		}

		return *it->second;
	}

	std::vector<std::unique_ptr<T>> begin() {
		return children.begin();
	}

	std::vector<std::unique_ptr<T>> end() {
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

class SetSymbol : public Symbol {
	SGVISITOR_ACCEPT
};

class ProgramSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	AST::Program* ast_program;
};

class RecordMemberSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::string Name;
	std::unique_ptr<Type> InputType;
	unsigned int Offset;

	RecordMemberSymbol(const std::string& Name, std::unique_ptr<Type>& InputType);
};

class RecordSymbol : public ParentSymbol<RecordMemberSymbol> {
	SGVISITOR_ACCEPT
public:
	AST::Record* ast_record;
	RecordMemberSymbol* QuantityMember;

	RecordSymbol();
};

class FunctionArgumentSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::string Name;
	std::unique_ptr<Type> InputType;
	unsigned int Offset;

	FunctionArgumentSymbol(const std::string& Name, std::unique_ptr<Type>& InputType);
};

class FunctionSymbol : public ParentSymbol<FunctionArgumentSymbol> {
	SGVISITOR_ACCEPT
public:
	AST::Function* ast_function;
};

class EnumerationMemberSymbol : public Symbol {
	SGVISITOR_ACCEPT
public:
	std::string Name;
	unsigned int Value;

	EnumerationMemberSymbol(const std::string& Name, unsigned int Value);
};

class EnumerationSymbol : public ParentSymbol<EnumerationMemberSymbol> {
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
