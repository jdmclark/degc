#include "function_visitor.h"
#include "function_identifier_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::FunctionVisitor;

FunctionVisitor::FunctionVisitor(size_t arg_ct, IR::Printer& code, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::FunctionVisitor", report), arg_ct(arg_ct), code(code) {
	return;
}

void FunctionVisitor::VisitIdentifierExpression(IdentifierExpression& e) {
	FunctionIdentifierVisitor v(arg_ct, code, Report);
	e.ReferencedNode->Accept(v);
}
