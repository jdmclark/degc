#pragma once

#include "printer.h"
#include "runtime/code/codebuffer.h"
#include "runtime/code/codebufferwritestream.h"
#include "runtime/code/functiontable.h"
#include "runtime/code/programtable.h"

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
	Runtime::Code::ProgramTable& programTable;

	std::unordered_map<std::string, size_t> labelmap;
	std::unordered_multimap<std::string, size_t> backpatchmap;

public:
	CodePrinter(Runtime::Code::CodeBuffer& codeBuffer, Runtime::Code::FunctionTable& functionTable, Runtime::Code::ProgramTable& programTable);

	void Backpatch();

	void Comment(const std::string& msg);
	void Label(const std::string& name);
	void Function(const std::string& universal_name);
	std::string GenerateUniqueLabel();

	void Nop();
	void Panic();

	void Call(const std::string& universal_name, size_t arg_ct);
	void CallS(size_t offset, size_t arg_ct);
	void Ret();

	void ConstB(bool value);
	void ConstN(Runtime::Math::DefaultFixed value);
	void ConstF(const std::string& universal_name);
	void ConstP(const std::string& universal_name);

	void LoadS(size_t offset);

	void MemB(size_t index);
	void MemN(size_t index);

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

	void Cons(size_t record_width);
	void ConsGt(size_t record_width, size_t element);
	void ConsGeq(size_t record_width, size_t element);
	void ConsLt(size_t record_width, size_t element);
	void ConsLeq(size_t record_width, size_t element);
	void ConsEq(size_t record_width, size_t element);
	void ConsNeq(size_t record_width, size_t element);
	void Union();
	void Intersect();
	void SetMinus();
	void Exists(size_t record_id);
};

}
}
}
