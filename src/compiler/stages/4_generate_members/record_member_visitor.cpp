#include "record_member_visitor.h"
#include "typename_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateMembers::RecordMemberVisitor;

RecordMemberVisitor::RecordMemberVisitor(SG::RecordSymbol& symbol, SG::Module& module, Diagnostics::Report& report)
	: AST::Visitor("GenerateMembers::RecordMemberVisitor", report), symbol(symbol), module(module) {
	return;
}

void RecordMemberVisitor::VisitRecordMember(RecordMember& member) {
	if(symbol.IsMember(member.Name)) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, member.Location, member.Name);
		return;
	}

	TypenameVisitor v(module, Report, true);
	member.Type->Accept(v);
	symbol.MakeMember<SG::RecordMemberSymbol>(member.Name, v.TypenameType);
	if(v.IsQuantityType) {
		if(symbol.QuantityMember != nullptr) {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::RecordMultipleQuantity, Diagnostics::ErrorLevel::Error,
					VisitorName, "record has multiple quantities", member.Location));
		}

		symbol.QuantityMember = &symbol.GetMember(member.Name);
	}
}
