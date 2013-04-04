#include "declaration_visitor.h"
#include <boost/format.hpp>

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateSG::DeclarationVisitor;

DeclarationVisitor::DeclarationVisitor(const std::string& module_name, Deg::Compiler::SG::Module& module, Diagnostics::Report& report)
	: AST::Visitor("GenerateSG::DeclarationVisitor", report), module_name(module_name), module(module) {
	return;
}

void DeclarationVisitor::VisitEnumeration(Enumeration& n) {
	try {
		auto& sym = module.MakeSymbol<SG::EnumerationSymbol>(n.Name);
		sym.ast_enumeration = &n;
	}
	catch(...) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
	}
}

void DeclarationVisitor::VisitRecord(Record& n) {
	try {
		auto& sym = module.MakeSymbol<SG::RecordSymbol>(n.Name);
		sym.ast_record = &n;
		sym.UniversalUniqueName = boost::str(boost::format("%s.%s") % module_name % n.Name);
	}
	catch(...) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
	}
}

void DeclarationVisitor::VisitFunction(Function& n) {
	try {
		auto& sym = module.MakeSymbol<SG::FunctionSymbol>(n.Name);
		sym.ast_function = &n;
		sym.UniversalUniqueName = boost::str(boost::format("%s.%s") % module_name % n.Name);
	}
	catch(...) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
	}
}

void DeclarationVisitor::VisitProgram(Program& n) {
	try {
		auto& sym = module.MakeSymbol<SG::ProgramSymbol>(n.Name);
		sym.ast_program = &n;
		sym.UniversalUniqueName = boost::str(boost::format("%s.%s") % module_name % n.Name);
	}
	catch(...) {
		SG::ErrorHelper::SymbolRedefinition(Report, VisitorName, n.Location, n.Name);
	}
}
