#include "declaration_visitor.h"
#include "enumeration_member_visitor.h"
#include "record_member_visitor.h"
#include "function_argument_visitor.h"
#include "program_argument_visitor.h"
#include "program_base_visitor.h"
#include "typename_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateMembers::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(Module& module, Runtime::Code::RecordTypeTable& recordTypeTable, Deg::Compiler::Diagnostics::Report& report)
	: SG::Visitor("GenerateMembers::DeclarationVisitor", report), module(module), recordTypeTable(recordTypeTable) {
	return;
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	if(n.ast_program->Extends.empty()) {
		n.Base = nullptr;
	}
	else {
		try {
			SG::Node& baseSymbol = module.GetSymbol(n.ast_program->Extends);
			ProgramBaseVisitor v(n, Report);
			baseSymbol.Accept(v);
		}
		catch(...) {
			n.Base = nullptr;
			SG::ErrorHelper::UndefinedSymbol(Report, VisitorName, n.ast_program->Location, n.ast_program->Extends);
		}
	}

	ProgramArgumentVisitor v(n, module, Report);
	for(auto member : *n.ast_program->Arguments) {
		member->Accept(v);
	}
	return;
}

void DeclarationVisitor::VisitEnumerationSymbol(EnumerationSymbol& n) {
	EnumerationMemberVisitor v(n, Report);
	for(auto member : *n.ast_enumeration->Members) {
		member->Accept(v);
	}
	return;
}

void DeclarationVisitor::VisitFunctionSymbol(FunctionSymbol& n) {
	TypenameVisitor codomain_v(module, Report);
	n.ast_function->Codomain->Accept(codomain_v);
	n.CodomainType = std::move(codomain_v.TypenameType);

	FunctionArgumentVisitor v(n, module, Report);
	for(auto member : *n.ast_function->Arguments) {
		member->Accept(v);
	}
	return;
}

void DeclarationVisitor::VisitRecordSymbol(RecordSymbol& n) {
	RecordMemberVisitor v(n, module, Report);
	for(auto member : *n.ast_record->Members) {
		member->Accept(v);
	}

	if(n.QuantityMember) {
		SG::RecordMemberSymbol* rms = dynamic_cast<SG::RecordMemberSymbol*>(n.QuantityMember);
		if(rms) {
			recordTypeTable.AddRecordType(n.UniversalUniqueName, n.Members.children_size(), rms->Index);
			return;
		}
	}

	recordTypeTable.AddRecordType(n.UniversalUniqueName, n.Members.children_size(), 0);
	return;
}
