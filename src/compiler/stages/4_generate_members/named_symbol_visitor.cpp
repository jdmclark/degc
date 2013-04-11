#include "named_symbol_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateMembers::NamedSymbolVisitor;

NamedSymbolVisitor::NamedSymbolVisitor(Diagnostics::Report& report)
	: SG::Visitor("GenerateMembers::NamedSymbolVisitor", report), TypenameType(new ErrorType()), IsQuantityType(false) {
	return;
}

void NamedSymbolVisitor::VisitNumberSymbol(NumberSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new NumberType());
}

void NamedSymbolVisitor::VisitQuantitySymbol(QuantitySymbol& n) {
	TypenameType = std::unique_ptr<Type>(new NumberType());
	IsQuantityType = true;
}

void NamedSymbolVisitor::VisitBooleanSymbol(BooleanSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new BooleanType());
}

void NamedSymbolVisitor::VisitRecordSymbol(RecordSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new RecordType(&n));
}

void NamedSymbolVisitor::VisitEnumerationSymbol(EnumerationSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new EnumerationType(&n));
}

void NamedSymbolVisitor::VisitProgramSymbol(ProgramSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new ProgramType(&n));
}
