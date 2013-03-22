#include "virtualmachine.h"
#include "runtime/code/codebufferreadstream.h"
#include "runtime/math/fixed.h"
#include "opcode.h"

using Deg::Runtime::Math::DefaultFixed;

Deg::Runtime::VM::VirtualMachine::VirtualMachine(const Code::CodeBuffer& code)
	: code(code) {
	return;
}

void Deg::Runtime::VM::VirtualMachine::Execute(size_t pc) {
	Code::CodeBufferReadStream stream(code);
	stream.Seek(pc);

	while(true) {
		Opcode op = stream.Read<Opcode>();

		switch(op) {
		case Opcode::NOP:
			break;

		case Opcode::CONSTB: {
				bool v = stream.Read<bool>();
				stack.Push(v);
			}
			break;

		case Opcode::CONSTN: {
				DefaultFixed v = stream.Read<DefaultFixed>();
				stack.Push(v);
			}
			break;

		case Opcode::NEG:
			stack.Push<DefaultFixed>(-stack.Pop<DefaultFixed>());
			break;

		case Opcode::ADD: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs + rhs);
			}
			break;

		case Opcode::SUB: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs - rhs);
			}
			break;

		case Opcode::MUL: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs * rhs);
			}
			break;

		case Opcode::DIV: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs / rhs);
			}
			break;

		case Opcode::CGT: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs > rhs);
			}
			break;

		case Opcode::CGEQ: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs >= rhs);
			}
			break;

		case Opcode::CLT: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs < rhs);
			}
			break;

		case Opcode::CLEQ: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs <= rhs);
			}
			break;

		case Opcode::CEQ: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs == rhs);
			}
			break;

		case Opcode::CNEQ: {
				DefaultFixed rhs = stack.Pop<DefaultFixed>();
				DefaultFixed lhs = stack.Pop<DefaultFixed>();
				stack.Push(lhs != rhs);
			}
			break;

		case Opcode::LNOT:
			stack.Push(!stack.Pop<bool>());
			break;

		case Opcode::LAND: {
				bool rhs = stack.Pop<bool>();
				bool lhs = stack.Pop<bool>();
				stack.Push(lhs && rhs);
			}
			break;

		case Opcode::LOR: {
				bool rhs = stack.Pop<bool>();
				bool lhs = stack.Pop<bool>();
				stack.Push(lhs || rhs);
			}
			break;
		}
	}
}
