#include "splitprinter.h"
#include "runtime/vm/opcode.h"
#include <boost/format.hpp>

using Deg::Compiler::IR::SplitPrinter;
using Deg::Runtime::VM::Opcode;

SplitPrinter::SplitPrinter(const std::vector<Printer*>& printers)
	: printers(printers), unique_label_id(0) {
	return;
}

void SplitPrinter::Backpatch() {
	for(auto p : printers) { p->Backpatch(); }
	return;
}

void SplitPrinter::Comment(const std::string& msg) {
	for(auto p : printers) { p->Comment(msg); }
	return;
}

void SplitPrinter::Label(const std::string& name) {
	for(auto p : printers) { p->Label(name); }
}

void SplitPrinter::Function(const std::string& universal_name) {
	for(auto p : printers) { p->Function(universal_name); }
}

std::string SplitPrinter::GenerateUniqueLabel() {
	for(auto p : printers) { p->GenerateUniqueLabel(); }
	return boost::str(boost::format("@L:%d") % unique_label_id++);
}

void SplitPrinter::Nop() {
	for(auto p : printers) { p->Nop(); }
}

void SplitPrinter::Panic() {
	for(auto p : printers) { p->Panic(); }
}

void SplitPrinter::Call(const std::string& universal_name, size_t arg_ct) {
	for(auto p : printers) { p->Call(universal_name, arg_ct); }
}

void SplitPrinter::CallS(size_t offset, size_t arg_ct) {
	for(auto p : printers) { p->CallS(offset, arg_ct); }
}

void SplitPrinter::Ret() {
	for(auto p : printers) { p->Ret(); }
}

void SplitPrinter::ConstB(bool value) {
	for(auto p : printers) { p->ConstB(value); }
}

void SplitPrinter::ConstN(Runtime::Math::DefaultFixed value) {
	for(auto p : printers) { p->ConstN(value); }
}

void SplitPrinter::ConstF(const std::string& universal_name) {
	for(auto p : printers) { p->ConstF(universal_name); }
}

void SplitPrinter::LoadS(size_t offset) {
	for(auto p : printers) { p->LoadS(offset); }
}

void SplitPrinter::MemB(size_t index) {
	for(auto p : printers) { p->MemB(index); }
}

void SplitPrinter::MemN(size_t index) {
	for(auto p : printers) { p->MemN(index); }
}

void SplitPrinter::LNot() {
	for(auto p : printers) { p->LNot(); }
}

void SplitPrinter::LAnd() {
	for(auto p : printers) { p->LAnd(); }
}

void SplitPrinter::LOr() {
	for(auto p : printers) { p->LOr(); }
}

void SplitPrinter::Neg() {
	for(auto p : printers) { p->Neg(); }
}

void SplitPrinter::Add() {
	for(auto p : printers) { p->Add(); }
}

void SplitPrinter::Sub() {
	for(auto p : printers) { p->Sub(); }
}

void SplitPrinter::Mul() {
	for(auto p : printers) { p->Mul(); }
}

void SplitPrinter::Div() {
	for(auto p : printers) { p->Div(); }
}

void SplitPrinter::CGt() {
	for(auto p : printers) { p->CGt(); }
}

void SplitPrinter::CGeq() {
	for(auto p : printers) { p->CGeq(); }
}

void SplitPrinter::CLt() {
	for(auto p : printers) { p->CLt(); }
}

void SplitPrinter::CLeq() {
	for(auto p : printers) { p->CLeq(); }
}

void SplitPrinter::CEq() {
	for(auto p : printers) { p->CEq(); }
}

void SplitPrinter::CNeq() {
	for(auto p : printers) { p->CNeq(); }
}

void SplitPrinter::Jmp(const std::string& label) {
	for(auto p : printers) { p->Jmp(label); }
}

void SplitPrinter::Btf(const std::string& t_label, const std::string& f_label) {
	for(auto p : printers) { p->Btf(t_label, f_label); }
}
