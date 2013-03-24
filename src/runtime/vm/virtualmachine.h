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
	typedef boost::variant<int, bool, Math::DefaultFixed, Math::Set, Solver::Record> Type;
	const Code::CodeBuffer& code;
	std::vector<Type> stack;
	int si;

	void Execute(size_t pc);

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

	template <typename T> T Call(int pc) {
		stack.push_back(0); // RV
		stack.push_back(0); // SI
		stack.push_back(-1); // PC, negative means C return
		si = 3;
		Execute(pc);
		return Pop<T>();
	}

	template <typename T, typename... args> T Call(int pc, args... arg) {
		stack.push_back(0); // RV
		stack.push_back(0); // SI
		stack.push_back(-1); // PC, negative means C return
		push_cons(arg...);
		si = 3;
		Execute(pc);
		return Pop<T>();
	}
};

}
}
}
