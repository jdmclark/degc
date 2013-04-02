#include "function_identifier_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::FunctionIdentifierVisitor;

FunctionIdentifierVisitor::FunctionIdentifierVisitor(size_t arg_ct, IR::Printer& code, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::FunctionIdentifierVisitor", report), arg_ct(arg_ct), code(code) {
	return;
}

void FunctionIdentifierVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	code.Call(n.UniversalUniqueName, arg_ct);
}

void FunctionIdentifierVisitor::VisitFunctionArgumentSymbol(FunctionArgumentSymbol& n) {
	code.CallS(n.Index, arg_ct);
}
