#include "stack.h"

Deg::Runtime::VM::Stack::Stack()
	: topPtr(0) {
	return;
}

void Deg::Runtime::VM::Stack::throwStackUnderrunException() const {
	throw std::exception();
}
