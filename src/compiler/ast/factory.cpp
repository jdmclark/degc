#include "factory.h"

using namespace Deg::Compiler::AST;

TranslationUnit* Factory::MakeTranslationUnit(const Diagnostics::ErrorLocation& yyl) {
	MAKE(TranslationUnit);
	return val;
}
