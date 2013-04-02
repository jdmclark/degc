#include "function_identifier_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::FunctionIdentifierVisitor;

FunctionIdentifierVisitor::FunctionIdentifierVisitor(IR::Printer& code, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::FunctionIdentifierVisitor", report), code(code) {
	return;
}

void FunctionIdentifierVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	code.Call(n.UniversalUniqueName, n.Arguments.children_size());
}
