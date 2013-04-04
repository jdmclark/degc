#include "declaration_visitor.h"
#include "expression_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::DeclarationVisitor", report), code(code), recordTypeTable(recordTypeTable) {
	return;
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	code.Function(n.UniversalUniqueName);
	ExpressionVisitor v(code, recordTypeTable, Report);
	n.Code->Accept(v);
	code.Ret();
}
