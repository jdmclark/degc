#include "typename_visitor.h"
#include "named_symbol_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateMembers::TypenameVisitor;

TypenameVisitor::TypenameVisitor(SG::Module& module, Diagnostics::Report& report, bool can_take_quantity)
	: AST::Visitor("GenerateMembers::TypenameVisitor", report), module(module), can_take_quantity(can_take_quantity),
	  TypenameType(new SG::ErrorType()), IsQuantityType(false) {
	return;
}

void TypenameVisitor::VisitNamedTypename(NamedTypename& n) {
	try {
		SG::Node& type_symbol = module.GetSymbol(n.Value);
		NamedSymbolVisitor v(Report);
		type_symbol.Accept(v);
		TypenameType = std::move(v.TypenameType);
		IsQuantityType = v.IsQuantityType;
	}
	catch(...) {
		SG::ErrorHelper::UndefinedTypename(Report, VisitorName, n.Location, n.Value);
	}

	if(!can_take_quantity && IsQuantityType) {
		SG::ErrorHelper::MeaninglessQuantity(Report, VisitorName, n.Location);
	}
}

void TypenameVisitor::VisitSetTypename(SetTypename& n) {
	try {
		SG::Node* type_symbol = &module.GetSymbol(n.RecordType);
		SG::RecordSymbol* record_symbol = dynamic_cast<SG::RecordSymbol*>(type_symbol);
		if(record_symbol) {
			TypenameType = std::unique_ptr<SG::Type>(new SG::SetType(record_symbol));
		}
		else {
			SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.RecordType);
		}
	}
	catch(...) {
		SG::ErrorHelper::UndefinedTypename(Report, VisitorName, n.Location, n.RecordType);
	}
}

void TypenameVisitor::VisitFunctionTypename(FunctionTypename& n) {
	std::unique_ptr<SG::FunctionType> fntype(new SG::FunctionType());

	TypenameVisitor codomain_v(module, Report);
	n.CodomainType->Accept(codomain_v);
	fntype->ReturnType = std::move(codomain_v.TypenameType);

	for(auto arg : *n.DomainType) {
		TypenameVisitor domain_v(module, Report);
		arg->Accept(domain_v);
		fntype->ArgumentTypes.push_back(std::move(domain_v.TypenameType));
	}

	TypenameType = std::move(fntype);
}
