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
	size_t unique_label_id;
	
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
	void Function(const std::string& universal_name);
	std::string GenerateUniqueLabel();

	void Nop();
	void Panic();

	void Ret();

	void ConstB(bool value);
	void ConstN(Runtime::Math::DefaultFixed value);

	void LoadS(int offset);
	void StoreS(int offset);

	void LNot();
	void LAnd();
	void LOr();

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

	void Jmp(const std::string& label);
	void Btf(const std::string& t_label, const std::string& f_label);
};

}
}
}
