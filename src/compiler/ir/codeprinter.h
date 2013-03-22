#pragma once

#include "printer.h"
#include "runtime/code/codebuffer.h"
#include "runtime/code/codebufferwritestream.h"
#include "runtime/code/functiontable.h"

#include <iostream>
#include <unordered_map>

namespace Deg {
namespace Compiler {
namespace IR {

class CodePrinter : public Printer {
private:
	Runtime::Code::CodeBuffer& codeBuffer;
	Runtime::Code::CodeBufferWriteStream stream;
	Runtime::Code::FunctionTable& functionTable;

	std::unordered_map<std::string, size_t> labelmap;
	std::unordered_multimap<std::string, size_t> backpatchmap;

public:
	CodePrinter(Runtime::Code::CodeBuffer& codeBuffer, Runtime::Code::FunctionTable& functionTable);

	void Backpatch();

	void Comment(const std::string& msg);
	void Label(const std::string& name);

	void Nop();

	void ConstB(bool value);
	void ConstN(Runtime::Math::DefaultFixed value);

	void Neg();
	void Add();
	void Sub();
	void Mul();
	void Div();

	void CGt();
	void CGeq();
	void CLt();
	void CLeq();
	void CEq();
	void CNeq();

	void LNot();
	void LAnd();
	void LOr();
};

}
}
}
