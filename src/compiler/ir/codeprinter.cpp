#include "codeprinter.h"
#include "runtime/vm/opcode.h"

using Deg::Compiler::IR::CodePrinter;
using Deg::Runtime::VM::Opcode;

CodePrinter::CodePrinter(Deg::Runtime::Code::CodeBuffer& codeBuffer, Deg::Runtime::Code::FunctionTable& functionTable)
	: codeBuffer(codeBuffer), stream(codeBuffer), functionTable(functionTable) {
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

void CodePrinter::Nop() {
	stream.Write(Opcode::NOP);
}

void CodePrinter::ConstB(bool value) {
	stream.Write(Opcode::CONSTB);
	stream.Write(value);
}

void CodePrinter::ConstN(Runtime::Math::DefaultFixed value) {
	stream.Write(Opcode::CONSTN);
	stream.Write(value);
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

void CodePrinter::LNot() {
	stream.Write(Opcode::LNOT);
}

void CodePrinter::LAnd() {
	stream.Write(Opcode::LAND);
}

void CodePrinter::LOr() {
	stream.Write(Opcode::LOR);
}
