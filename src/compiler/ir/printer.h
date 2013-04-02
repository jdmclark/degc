#pragma once

#include <string>
#include "runtime/math/fixed.h"

namespace Deg {
namespace Compiler {
namespace IR {

class Printer {
public:
	virtual ~Printer();

	virtual void Backpatch() = 0;

	virtual void Comment(const std::string& msg) = 0;
	virtual void Label(const std::string& name) = 0;
	virtual void Function(const std::string& universal_name) = 0;
	virtual std::string GenerateUniqueLabel() = 0;

	virtual void Nop() = 0;
	virtual void Panic() = 0;

	virtual void Call(const std::string& universal_name, size_t arg_ct) = 0;
	virtual void CallS(size_t offset, size_t arg_ct) = 0;
	virtual void Ret() = 0;

	virtual void ConstB(bool value) = 0;
	virtual void ConstN(Runtime::Math::DefaultFixed value) = 0;
	virtual void ConstF(const std::string& universal_name) = 0;

	virtual void LoadS(size_t offset) = 0;
	virtual void StoreS(size_t offset) = 0;

	virtual void LNot() = 0;
	virtual void LAnd() = 0;
	virtual void LOr() = 0;

	virtual void Neg() = 0;
	virtual void Add() = 0;
	virtual void Sub() = 0;
	virtual void Mul() = 0;
	virtual void Div() = 0;

	virtual void CGt() = 0;
	virtual void CGeq() = 0;
	virtual void CLt() = 0;
	virtual void CLeq() = 0;
	virtual void CEq() = 0;
	virtual void CNeq() = 0;

	virtual void Jmp(const std::string& label) = 0;
	virtual void Btf(const std::string& t_label, const std::string& f_label) = 0;
};

}
}
}
