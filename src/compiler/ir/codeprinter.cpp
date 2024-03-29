#include "codeprinter.h"
#include "runtime/vm/opcode.h"
#include <boost/format.hpp>

using Deg::Compiler::IR::CodePrinter;
using Deg::Runtime::VM::Opcode;

CodePrinter::CodePrinter(Deg::Runtime::Code::CodeBuffer& codeBuffer, Deg::Runtime::Code::FunctionTable& functionTable, Deg::Runtime::Code::ProgramTable& programTable)
	: unique_label_id(0), codeBuffer(codeBuffer), stream(codeBuffer), functionTable(functionTable), programTable(programTable) {
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

void CodePrinter::Assert(int path) {
	stream.Write(Opcode::ASSERT);
	stream.Write<int>(path);
}

void CodePrinter::Call(const std::string& universal_name, size_t arg_ct) {
	stream.Write(Opcode::CALL);
	backpatchmap.insert(std::make_pair(boost::str(boost::format("@FUNCTION:%s") % universal_name), stream.Tell()));
	stream.Write<size_t>(0);
	stream.Write(arg_ct);
}

void CodePrinter::CallS(size_t offset, size_t arg_ct) {
	stream.Write(Opcode::CALLS);
	stream.Write(offset);
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

void CodePrinter::ConstF(const std::string& universal_name) {
	stream.Write(Opcode::CONSTF);
	backpatchmap.insert(std::make_pair(boost::str(boost::format("@FUNCTION:%s") % universal_name), stream.Tell()));
	stream.Write<size_t>(0);
}

void CodePrinter::ConstP(const std::string& universal_name) {
	stream.Write(Opcode::CONSTP);
	stream.Write<size_t>(programTable.GetProgramId(universal_name));
}

void CodePrinter::LoadS(size_t offset) {
	stream.Write(Opcode::LOADS);
	stream.Write(offset);
}

void CodePrinter::MemB(size_t index) {
	stream.Write(Opcode::MEMB);
	stream.Write(index);
}

void CodePrinter::MemN(size_t index) {
	stream.Write(Opcode::MEMN);
	stream.Write(index);
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

void CodePrinter::Cons(size_t record_width) {
	stream.Write(Opcode::CONS);
	stream.Write<size_t>(record_width);
}

void CodePrinter::ConsGt(size_t record_width, size_t element) {
	stream.Write(Opcode::CONSGT);
	stream.Write<size_t>(record_width);
	stream.Write<size_t>(element);
}

void CodePrinter::ConsGeq(size_t record_width, size_t element) {
	stream.Write(Opcode::CONSGEQ);
	stream.Write<size_t>(record_width);
	stream.Write<size_t>(element);
}

void CodePrinter::ConsLt(size_t record_width, size_t element) {
	stream.Write(Opcode::CONSLT);
	stream.Write<size_t>(record_width);
	stream.Write<size_t>(element);
}

void CodePrinter::ConsLeq(size_t record_width, size_t element) {
	stream.Write(Opcode::CONSLEQ);
	stream.Write<size_t>(record_width);
	stream.Write<size_t>(element);
}

void CodePrinter::ConsEq(size_t record_width, size_t element) {
	stream.Write(Opcode::CONSEQ);
	stream.Write<size_t>(record_width);
	stream.Write<size_t>(element);
}

void CodePrinter::ConsNeq(size_t record_width, size_t element) {
	stream.Write(Opcode::CONSNEQ);
	stream.Write<size_t>(record_width);
	stream.Write<size_t>(element);
}

void CodePrinter::Union() {
	stream.Write(Opcode::UNION);
}

void CodePrinter::Intersect() {
	stream.Write(Opcode::INTERSECT);
}

void CodePrinter::SetMinus() {
	stream.Write(Opcode::SETMINUS);
}

void CodePrinter::Exists(size_t record_id) {
	stream.Write(Opcode::EXISTS);
	stream.Write<size_t>(record_id);
}
