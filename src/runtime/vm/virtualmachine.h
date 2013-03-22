#pragma once

#include "runtime/code/codebuffer.h"
#include "runtime/vm/stack.h"
#include <vector>

namespace Deg {
namespace Runtime {
namespace VM {

class VirtualMachine {
private:
	const Code::CodeBuffer& code;

public:
	Stack stack;

	VirtualMachine(const Code::CodeBuffer& code);

	void Execute(size_t pc);
};

}
}
}
