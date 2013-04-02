#include "function_visitor.h"
#include "function_identifier_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::FunctionVisitor;

FunctionVisitor::FunctionVisitor(IR::Printer& code, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::FunctionVisitor", report), code(code) {
	return;
}

void FunctionVisitor::VisitIdentifierExpression(IdentifierExpression& e) {
	FunctionIdentifierVisitor v(code, Report);
	e.ReferencedNode->Accept(v);
}
