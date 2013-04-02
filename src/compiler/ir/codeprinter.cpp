#include "codeprinter.h"
#include "runtime/vm/opcode.h"
#include <boost/format.hpp>

using Deg::Compiler::IR::CodePrinter;
using Deg::Runtime::VM::Opcode;

CodePrinter::CodePrinter(Deg::Runtime::Code::CodeBuffer& codeBuffer, Deg::Runtime::Code::FunctionTable& functionTable)
	: unique_label_id(0), codeBuffer(codeBuffer), stream(codeBuffer), functionTable(functionTable) {
	return;
}

void CodePrinter::Backpatch() {
	for(auto it = backpatchmap.begin(); it != backpatchmap.end(); ++it) {
		auto jt = labelmap.find(it->first);
		if(jt != labelmap.end()) {
			codeBuffer.Write<size_t>(jt->second, it->second);
		}
		else {
			// Missing labels already reported by semantic analysis.
			// Default to zero.
			codeBuffer.Write<size_t>(0, it->second);
		}
	}
}

void CodePrinter::Comment(const std::string& msg) {
	return;
}

void CodePrinter::Label(const std::string& name) {
	labelmap.insert(std::make_pair(name, stream.Tell()));
}

void CodePrinter::Function(const std::string& universal_name) {
	Label(boost::str(boost::format("@FUNCTION:%s") % universal_name));
	functionTable.AddFunction(universal_name, stream.Tell());
}

std::string CodePrinter::GenerateUniqueLabel() {
	return boost::str(boost::format("@L:%d") % unique_label_id++);
}

void CodePrinter::Nop() {
	stream.Write(Opcode::NOP);
}

void CodePrinter::Panic() {
	stream.Write(Opcode::PANIC);
}

void CodePrinter::Call(const std::string& universal_name, size_t arg_ct) {
	stream.Write(Opcode::CALL);
	backpatchmap.insert(std::make_pair(boost::str(boost::format("@FUNCTION:%s") % universal_name), stream.Tell()));
	stream.Write<size_t>(0);
	stream.Write(arg_ct);
}

void CodePrinter::Ret() {
	stream.Write(Opcode::RET);
}

void CodePrinter::ConstB(bool value) {
	stream.Write(Opcode::CONSTB);
	stream.Write(value);
}

void CodePrinter::ConstN(Runtime::Math::DefaultFixed value) {
	stream.Write(Opcode::CONSTN);
	stream.Write(value);
}

void CodePrinter::LoadS(size_t offset) {
	stream.Write(Opcode::LOADS);
	stream.Write(offset);
}

void CodePrinter::StoreS(size_t offset) {
	stream.Write(Opcode::STORES);
	stream.Write(offset);
}

void CodePrinter::LNot() {
	stream.Write(Opcode::LNOT);
}

void CodePrinter::LAnd() {
	stream.Write(Opcode::LAND);
}

void CodePrinter::LOr() {
	stream.Write(Opcode::LOR);
}

void CodePrinter::Neg() {
	stream.Write(Opcode::NEG);
}

void CodePrinter::Add() {
	stream.Write(Opcode::ADD);
}

void CodePrinter::Sub() {
	stream.Write(Opcode::SUB);
}

void CodePrinter::Mul() {
	stream.Write(Opcode::MUL);
}

void CodePrinter::Div() {
	stream.Write(Opcode::DIV);
}

void CodePrinter::CGt() {
	stream.Write(Opcode::CGT);
}

void CodePrinter::CGeq() {
	stream.Write(Opcode::CGEQ);
}

void CodePrinter::CLt() {
	stream.Write(Opcode::CLT);
}

void CodePrinter::CLeq() {
	stream.Write(Opcode::CLEQ);
}

void CodePrinter::CEq() {
	stream.Write(Opcode::CEQ);
}

void CodePrinter::CNeq() {
	stream.Write(Opcode::CNEQ);
}

void CodePrinter::Jmp(const std::string& label) {
	stream.Write(Opcode::JMP);
	backpatchmap.insert(std::make_pair(label, stream.Tell()));
	stream.Write<size_t>(0);
}

void CodePrinter::Btf(const std::string& t_label, const std::string& f_label) {
	stream.Write(Opcode::BTF);
	backpatchmap.insert(std::make_pair(t_label, stream.Tell()));
	stream.Write<size_t>(0);
	backpatchmap.insert(std::make_pair(f_label, stream.Tell()));
	stream.Write<size_t>(0);
}
