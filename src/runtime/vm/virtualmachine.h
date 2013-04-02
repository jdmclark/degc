#pragma once

#include "runtime/code/codebuffer.h"
#include "runtime/math/fixed.h"
#include "runtime/math/set.h"
#include "runtime/solver/record.h"
#include <vector>
#include <boost/variant.hpp>
#include <boost/variant/get.hpp>

namespace Deg {
namespace Runtime {
namespace VM {

class VirtualMachine {
private:
	typedef boost::variant<bool, Math::DefaultFixed, Math::Set, Solver::Record> Type;
	const Code::CodeBuffer& code;
	std::vector<Type> stack;
	std::vector<size_t> si_stack;
	std::vector<size_t> pc_stack;
	size_t si;

	Type Execute(size_t pc);

	template <typename head> void push_cons(head head_v) {
		stack.push_back(head_v);
	}

	template <typename head, typename... tail> void push_cons(head head_v, tail... tail_v) {
		stack.push_back(head_v);
		push_cons(tail_v...);
	}

public:
	VirtualMachine(const Code::CodeBuffer& code);

	template <typename T> inline void Push(const T& value) {
		stack.push_back(value);
	}

	template <typename T> inline T Pop() {
		Type rv = stack.back();
		stack.pop_back();
		return boost::get<T>(rv);
	}

	template <typename T> T Call(size_t pc) {
		stack.clear();
		si_stack.clear();
		pc_stack.clear();
		return boost::get<T>(Execute(pc));
	}

	template <typename T, typename... args> T Call(size_t pc, args... arg) {
		stack.clear();
		si_stack.clear();
		pc_stack.clear();
		push_cons(arg...);
		return boost::get<T>(Execute(pc));
	}
};

}
}
}
