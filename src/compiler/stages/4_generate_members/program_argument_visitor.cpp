#include "program_argument_visitor.h"
#include "typename_visitor.h"
#include "compiler/sg/error_helper.h"
#include <boost/format.hpp>

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateMembers::ProgramArgumentVisitor;

ProgramArgumentVisitor::ProgramArgumentVisitor(SG::ProgramSymbol& symbol, SG::Module& module, Diagnostics::Report& report)
	: AST::Visitor("GenerateMembers::ProgramArgumentVisitor", report), symbol(symbol), module(module) {
	return;
}

void ProgramArgumentVisitor::VisitFunctionArgument(FunctionArgument& n) {
	if(symbol.Arguments.IsMember(n.Name)) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
		return;
	}

	TypenameVisitor v(module, Report);
	n.Type->Accept(v);

	SG::EnumerationType* et = dynamic_cast<SG::EnumerationType*>(v.TypenameType.get());
	if(!et) {
		// Error: Program arguments must be enumeration type.
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ProgArgNotEnumeration, Diagnostics::ErrorLevel::Error,
				VisitorName, boost::str(boost::format("program argument %s is not enumeration type") % n.Name), n.Location));
	}

	symbol.Arguments.MakeMember<SG::ProgramArgumentSymbol>(n.Name, v.TypenameType, symbol.Arguments.children_size());
}
