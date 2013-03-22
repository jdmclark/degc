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

	virtual void Nop() = 0;

	virtual void ConstB(bool value) = 0;
	virtual void ConstN(Runtime::Math::DefaultFixed value) = 0;

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

	virtual void LNot() = 0;
	virtual void LAnd() = 0;
	virtual void LOr() = 0;
};

}
}
}
