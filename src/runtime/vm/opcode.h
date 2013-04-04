#pragma once

namespace Deg {
namespace Runtime {
namespace VM {

enum class Opcode : unsigned char {
	NOP,
	PANIC,

	CALL,					// Calls a static function
	CALLS,					// Calls the function stored at the stack offset
	RET,					// Returns value to calling stack frame

	CONSTB,					// Load constant boolean
	CONSTN,					// Load constant numeric
	CONSTF,					// Load constant function reference

	LOADS,					// Load from stack offset

	MEMB,					// Load record boolean member
	MEMN,					// Load record numeric member

	// Boolean operations
	LNOT,					// Negate top
	LAND,					// Top && second-top
	LOR,					// Top || second-top

	// Numeric operations
	NEG,					// Negate top numeric from stack
	ADD,					// Add top two
	SUB,					// Subtract top from second-top
	MUL,					// Multiply top two
	DIV,					// Divide top from second-top

	// Numeric comparisons
	CGT,					// Second-top > top
	CGEQ,					// Second-top <= top
	CLT,					// Second-top < top
	CLEQ,					// Second-top <= top
	CEQ,					// Second-top == top
	CNEQ,					// Second-top != top

	// Branch operations
	JMP,					// Jump to target PC
	BTF,					// Branch to first if true, second if false

	// Set operations
	CONS,					// Construct unconstrained set
	CONSGT,
	CONSGEQ,
	CONSLT,
	CONSLEQ,
	CONSEQ,
	CONSNEQ,
	UNION,
	INTERSECT,
	SETMINUS,
	EXISTS					// Checks if elements of specified set exist
};

}
}
}
