#include "virtualmachine.h"
#include "runtime/code/codebufferreadstream.h"
#include "runtime/math/fixed.h"
#include "opcode.h"

using Deg::Runtime::Math::DefaultFixed;

Deg::Runtime::VM::VirtualMachine::VirtualMachine(const Code::CodeBuffer& code, const Solver::RecordIndex& record_index)
	: code(code), record_index(record_index), si(0), rejects(nullptr) {
	return;
}

Deg::Runtime::VM::VirtualMachine::Type Deg::Runtime::VM::VirtualMachine::Execute(size_t pc) {
	Code::CodeBufferReadStream stream(code);
	stream.Seek(pc);

	while(true) {
		Opcode op = stream.Read<Opcode>();

		switch(op) {
		case Opcode::NOP:
			break;

		case Opcode::PANIC:
			throw std::exception();

		case Opcode::ASSERT: {
				int path = stream.Read<int>();
				bool value = Pop<bool>();
				if(!value) {
					rejects->insert(path);
				}
			}
			break;

		case Opcode::CALL: {
				size_t tgt_pc = stream.Read<size_t>();
				size_t arg_ct = stream.Read<size_t>();

				pc_stack.push_back(stream.Tell());
				si_stack.push_back(si);

				si = stack.size() - arg_ct;
				stream.Seek(tgt_pc);
			}
			break;

		case Opcode::CALLS: {
				size_t fn_off = stream.Read<size_t>();
				size_t tgt_pc = boost::get<size_t>(stack[si + fn_off]);
				size_t arg_ct = stream.Read<size_t>();

				pc_stack.push_back(stream.Tell());
				si_stack.push_back(si);

				si = stack.size() - arg_ct;
				stream.Seek(tgt_pc);
			}
			break;

		case Opcode::RET:
			if(pc_stack.empty()) {
				return stack.back();
			}
			else {
				Type value = stack.back();
				size_t n_pc = pc_stack.back(); pc_stack.pop_back();
				size_t n_si = si_stack.back(); si_stack.pop_back();
				stack.resize(si);
				stream.Seek(n_pc);
				si = n_si;
				stack.push_back(value);
			}
			break;

		case Opcode::CONSTB: {
				bool v = stream.Read<bool>();
				Push(v);
			}
			break;

		case Opcode::CONSTN: {
				DefaultFixed v = stream.Read<DefaultFixed>();
				Push(v);
			}
			break;

		case Opcode::CONSTF:
		case Opcode::CONSTP: {
				size_t v = stream.Read<size_t>();
				Push(v);
			}
			break;

		case Opcode::LOADS: {
				size_t offset = stream.Read<size_t>();
				stack.push_back(stack[si + offset]);
			}
			break;

		case Opcode::MEMB: {
				size_t index = stream.Read<size_t>();
				Runtime::Solver::Record r = Pop<Runtime::Solver::Record>();
				if(r[index] != DefaultFixed(0)) {
					Push(true);
				}
				else {
					Push(false);
				}
			}
			break;

		case Opcode::MEMN: {
				size_t index = stream.Read<size_t>();
				Runtime::Solver::Record r = Pop<Runtime::Solver::Record>();
				Push(r[index]);
			}
			break;

		case Opcode::LNOT:
			Push(!Pop<bool>());
			break;

		case Opcode::LAND: {
				bool rhs = Pop<bool>();
				bool lhs = Pop<bool>();
				Push(lhs && rhs);
			}
			break;

		case Opcode::LOR: {
				bool rhs = Pop<bool>();
				bool lhs = Pop<bool>();
				Push(lhs || rhs);
			}
			break;

		case Opcode::NEG:
			Push<DefaultFixed>(-Pop<DefaultFixed>());
			break;

		case Opcode::ADD: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs + rhs);
			}
			break;

		case Opcode::SUB: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs - rhs);
			}
			break;

		case Opcode::MUL: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs * rhs);
			}
			break;

		case Opcode::DIV: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs / rhs);
			}
			break;

		case Opcode::CGT: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs > rhs);
			}
			break;

		case Opcode::CGEQ: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs >= rhs);
			}
			break;

		case Opcode::CLT: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs < rhs);
			}
			break;

		case Opcode::CLEQ: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs <= rhs);
			}
			break;

		case Opcode::CEQ: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs == rhs);
			}
			break;

		case Opcode::CNEQ: {
				DefaultFixed rhs = Pop<DefaultFixed>();
				DefaultFixed lhs = Pop<DefaultFixed>();
				Push(lhs != rhs);
			}
			break;

		case Opcode::JMP: {
				size_t pc = stream.Read<size_t>();
				stream.Seek(pc);
			}
			break;

		case Opcode::BTF: {
				bool value = Pop<bool>();
				size_t pct = stream.Read<size_t>();
				size_t pcf = stream.Read<size_t>();
				if(value) {
					stream.Seek(pct);
				}
				else {
					stream.Seek(pcf);
				}
			}
			break;

		case Opcode::CONS: {
				size_t record_width = stream.Read<size_t>();
				Push(Math::Set(record_width));
			}
			break;

		case Opcode::CONSGT: {
				size_t record_width = stream.Read<size_t>();
				size_t element = stream.Read<size_t>();
				Push(Math::Set(record_width, element, Math::Relation::Greater, Pop<DefaultFixed>()));
			}
			break;

		case Opcode::CONSGEQ: {
				size_t record_width = stream.Read<size_t>();
				size_t element = stream.Read<size_t>();
				Push(Math::Set(record_width, element, Math::Relation::GreaterEqual, Pop<DefaultFixed>()));
			}
			break;

		case Opcode::CONSLT: {
				size_t record_width = stream.Read<size_t>();
				size_t element = stream.Read<size_t>();
				Push(Math::Set(record_width, element, Math::Relation::Less, Pop<DefaultFixed>()));
			}
			break;

		case Opcode::CONSLEQ: {
				size_t record_width = stream.Read<size_t>();
				size_t element = stream.Read<size_t>();
				Push(Math::Set(record_width, element, Math::Relation::LessEqual, Pop<DefaultFixed>()));
			}
			break;

		case Opcode::CONSEQ: {
				size_t record_width = stream.Read<size_t>();
				size_t element = stream.Read<size_t>();
				Push(Math::Set(record_width, element, Math::Relation::Equal, Pop<DefaultFixed>()));
			}
			break;

		case Opcode::CONSNEQ: {
				size_t record_width = stream.Read<size_t>();
				size_t element = stream.Read<size_t>();
				Push(Math::Set(record_width, element, Math::Relation::NotEqual, Pop<DefaultFixed>()));
			}
			break;

		case Opcode::UNION: {
				Math::Set rhs = Pop<Math::Set>();
				Math::Set lhs = Pop<Math::Set>();
				Push(lhs | rhs);
			}
			break;

		case Opcode::INTERSECT: {
				Math::Set rhs = Pop<Math::Set>();
				Math::Set lhs = Pop<Math::Set>();
				Push(lhs & rhs);
			}
			break;

		case Opcode::SETMINUS : {
				Math::Set rhs = Pop<Math::Set>();
				Math::Set lhs = Pop<Math::Set>();
				Push(lhs - rhs);
			}
			break;

		case Opcode::EXISTS: {
				size_t record_type = stream.Read<size_t>();
				Math::Set set = Pop<Math::Set>();
				Runtime::Solver::RecordTable* rt = record_index.GetRecordTable(record_type);
				if(rt) {
					Push(!rt->IsEmpty(set));
				}
				else {
					Push(false);
				}
			}
			break;
		}
	}

	return false;
}
