#include "textprinter.h"
#include "runtime/vm/opcode.h"
#include <boost/format.hpp>
#include <iostream>

using Deg::Compiler::IR::TextPrinter;
using Deg::Runtime::VM::Opcode;

TextPrinter::TextPrinter(std::ostream& os)
	: os(os), unique_label_id(0) {
	return;
}

void TextPrinter::Backpatch() {
	return;
}

void TextPrinter::Comment(const std::string& msg) {
	os << "\t; " << msg << std::endl;
	return;
}

void TextPrinter::Label(const std::string& name) {
	os << name << ":" << std::endl;
}

void TextPrinter::Function(const std::string& universal_name) {
	os << boost::str(boost::format("@FUNCTION:%s") % universal_name) << ":" << std::endl;
}

std::string TextPrinter::GenerateUniqueLabel() {
	return boost::str(boost::format("@L:%d") % unique_label_id++);
}

void TextPrinter::Nop() {
	os << "\tNOP" << std::endl;
}

void TextPrinter::Panic() {
	os << "\tPANIC" << std::endl;
}

void TextPrinter::Call(const std::string& universal_name, size_t arg_ct) {
	os << "\tCALL(" << arg_ct << ") " << boost::str(boost::format("@FUNCTION:%s") % universal_name) << std::endl;
}

void TextPrinter::Ret() {
	os << "\tRET" << std::endl;
}

void TextPrinter::ConstB(bool value) {
	os << "\tCONSTB " << value << std::endl;
}

void TextPrinter::ConstN(Runtime::Math::DefaultFixed value) {
	os << "\tCONSTN " << static_cast<std::string>(value) << std::endl;
}

void TextPrinter::LoadS(size_t offset) {
	os << "\tLOADS " << offset << std::endl;
}

void TextPrinter::StoreS(size_t offset) {
	os << "\tSTORES " << offset << std::endl;
}

void TextPrinter::LNot() {
	os << "\tLNOT" << std::endl;
}

void TextPrinter::LAnd() {
	os << "\tLAND" << std::endl;
}

void TextPrinter::LOr() {
	os << "\tLOR" << std::endl;
}

void TextPrinter::Neg() {
	os << "\tNEG" << std::endl;
}

void TextPrinter::Add() {
	os << "\tADD" << std::endl;
}

void TextPrinter::Sub() {
	os << "\tSUB" << std::endl;
}

void TextPrinter::Mul() {
	os << "\tMUL" << std::endl;
}

void TextPrinter::Div() {
	os << "\tDIV" << std::endl;
}

void TextPrinter::CGt() {
	os << "\tCGT" << std::endl;
}

void TextPrinter::CGeq() {
	os << "\tCGEQ" << std::endl;
}

void TextPrinter::CLt() {
	os << "\tCLT" << std::endl;
}

void TextPrinter::CLeq() {
	os << "\tCLEQ" << std::endl;
}

void TextPrinter::CEq() {
	os << "\tCEQ" << std::endl;
}

void TextPrinter::CNeq() {
	os << "\tCNEQ" << std::endl;
}

void TextPrinter::Jmp(const std::string& label) {
	os << "\tJMP " << label << std::endl;
}

void TextPrinter::Btf(const std::string& t_label, const std::string& f_label) {
	os << "\tBTF " << t_label << " " << f_label << std::endl;
}
