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
	if(symbol.IsMember(n.Name)) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
		return;
	}

	try {
		SG::Symbol& type_symbol = module.GetSymbol(n.Typename);
		TypenameVisitor v(Report);
		type_symbol.Accept(v);
		symbol.MakeMember<SG::FunctionArgumentSymbol>(n.Name, v.TypenameType);
	}
	catch(...) {
		SG::ErrorHelper::UndefinedTypename(Report, VisitorName, n.Location, n.Typename);
		std::unique_ptr<SG::Type> errorType(new SG::ErrorType());
		symbol.MakeMember<SG::FunctionArgumentSymbol>(n.Name, errorType);
	}
}
