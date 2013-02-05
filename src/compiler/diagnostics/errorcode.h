#pragma once

#include <iosfwd>

namespace Deg {
namespace Compiler {
namespace Diagnostics {

enum class ErrorCode {
	// Internal system errors
	Internal					= 1000,
	FeatureNotImplemented		= 1001,
	FileNotFound				= 1002,

	// Tokenizer errors
	UnrecognizedInput			= 2000,

	// Syntax errors
	SyntaxError					= 3000,

	// Semantic errors
	ImportUndefinedModule		= 4000,
	ImportUndefinedSymbol		= 4001,
	SymbolRedefinition			= 4002,
	RecordMultipleQuantity		= 4003,
	UndefinedTypename			= 4004,
	UndefinedSymbol				= 4005,
	ProgramExpected				= 4006,
	RecordNameExpected			= 4007,
	MeaninglessQuantity			= 4008,
	CodomainTypeMismatch		= 4009,
	BooleanExpected				= 4010,
	NumberExpected				= 4011,
	SetExpected					= 4012,
	SetDomainMismatch			= 4013,
	SymbolNotExpression			= 4014,
	FunctionArgTypeMismatch		= 4015,
	FunctionArgCountMismatch	= 4016,
	FunctionCallToNonFunction	= 4017,
	ConditionalDomainMismatch	= 4018,
	ComparisonTypeMismatch		= 4019,
};

std::ostream& operator<<(std::ostream&, ErrorCode);

}
}
}
