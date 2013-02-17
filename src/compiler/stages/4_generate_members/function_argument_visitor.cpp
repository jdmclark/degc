#include "function_argument_visitor.h"
#include "typename_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateMembers::FunctionArgumentVisitor;

FunctionArgumentVisitor::FunctionArgumentVisitor(SG::FunctionSymbol& symbol, SG::Module& module, Diagnostics::Report& report)
	: AST::Visitor("GenerateMembers::FunctionArgumentVisitor", report), symbol(symbol), module(module) {
	return;
}

void FunctionArgumentVisitor::VisitFunctionArgument(FunctionArgument& n) {
	if(symbol.Arguments.IsMember(n.Name)) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
		return;
	}

	TypenameVisitor v(module, Report);
	n.Type->Accept(v);
	symbol.Arguments.MakeMember<SG::FunctionArgumentSymbol>(n.Name, v.TypenameType, symbol.Arguments.children_size());
}
