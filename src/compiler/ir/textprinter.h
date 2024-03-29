#pragma once

#include "printer.h"
#include <iosfwd>

namespace Deg {
namespace Compiler {
namespace IR {

class TextPrinter : public Printer {
private:
	std::ostream& os;
	size_t unique_label_id;

public:
	TextPrinter(std::ostream& os);

	void Backpatch();

	void Comment(const std::string& msg);
	void Label(const std::string& name);
	void Function(const std::string& universal_name);
	std::string GenerateUniqueLabel();

	void Nop();
	void Panic();

	void Assert(int path);

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
