#include "enumeration_member_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateMembers::EnumerationMemberVisitor;

EnumerationMemberVisitor::EnumerationMemberVisitor(Deg::Compiler::SG::EnumerationSymbol& symbol, Diagnostics::Report& report)
	: AST::Visitor("GenerateMembers::EnumerationMemberVisitor", report), symbol(symbol) {
	return;
}

void EnumerationMemberVisitor::VisitEnumerationMember(EnumerationMember& n) {
	if(symbol.IsMember(n.Name)) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
		return;
	}

	symbol.MakeMember<SG::EnumerationMemberSymbol>(n.Name, symbol.children_size());
}
