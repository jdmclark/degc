#include "declaration_visitor.h"
#include "enumeration_member_visitor.h"
#include "record_member_visitor.h"
#include "function_argument_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateMembers::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(Module& module, Deg::Compiler::Diagnostics::Report& report)
	: SG::Visitor("GenerateMembers::DeclarationVisitor", report), module(module) {
	return;
}

void DeclarationVisitor::VisitProgramSymbol(ProgramSymbol& n) {
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
	return;
}
