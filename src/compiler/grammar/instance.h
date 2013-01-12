#pragma once

#include <string>
#include <iosfwd>

#include "compiler/ast/factory.h"
#include "compiler/diagnostics/report.h"

namespace Deg {
namespace Compiler {
namespace Grammar {

class Instance {
private:
	std::istream& file;

	// Scanner initialization members.
	// Implemented in lexer.lex.
	void InitScanner();
	void DestroyScanner();

	void* scanner;

	int indentationLevel;
	int insideParentheses;

	AST::TranslationUnit* ReturnValue;

public:
	char const * const Filename;
	AST::Factory* const Factory;
	Diagnostics::Report& Report;
	int CurrentLineIndentation;

	Instance(std::istream& file, const std::string& filename, AST::Factory& ast, Diagnostics::Report& report);
	~Instance();

	char GetNext();
	AST::TranslationUnit* Parse();

	inline void* GetScanner() const {
		return scanner;
	}

	inline int GetIndentationLevel() const {
		return indentationLevel;
	}

	inline void PushIndentation() {
		++indentationLevel;
	}

	inline void PopIndentation() {
		--indentationLevel;
	}

	inline bool IsInsideParentheses() const {
		return insideParentheses > 0;
	}

	inline void PushParentheses() {
		++insideParentheses;
	}

	inline void PopParentheses() {
		--insideParentheses;
	}

	inline void SetReturnValue(AST::TranslationUnit* astroot) {
		ReturnValue = astroot;
	}
};

}
}
}
