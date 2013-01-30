#include "typename_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateMembers::TypenameVisitor;

TypenameVisitor::TypenameVisitor(Diagnostics::Report& report)
	: SG::Visitor("GenerateMembers::TypenameVisitor", report), TypenameType(new ErrorType()), IsQuantityType(false) {
	return;
}

void TypenameVisitor::VisitNumberSymbol(NumberSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new NumberType());
}

void TypenameVisitor::VisitQuantitySymbol(QuantitySymbol& n) {
	TypenameType = std::unique_ptr<Type>(new NumberType());
	IsQuantityType = true;
}

void TypenameVisitor::VisitBooleanSymbol(BooleanSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new BooleanType());
}

void TypenameVisitor::VisitSetSymbol(SetSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new SetType());
}

void TypenameVisitor::VisitRecordSymbol(RecordSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new RecordType(&n));
}

void TypenameVisitor::VisitEnumerationSymbol(EnumerationSymbol& n) {
	TypenameType = std::unique_ptr<Type>(new EnumerationType(&n));
}
