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

	TranslationUnit* MakeTranslationUnit(const Diagnostics::ErrorLocation& yyl);
};

}
}
}
