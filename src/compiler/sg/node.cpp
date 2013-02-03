#include "node.h"
#include "visitor.h"

Deg::Compiler::SG::Node::~Node() {
	return;
}

#define SGVISITOR_ACCEPT_IMPL(x)						\
	void Deg::Compiler::SG::x::Accept(Visitor& v) {		\
		v.Visit##x(*this);								\
		return;											\
	}

Deg::Compiler::SG::ConstrainedSetType::ConstrainedSetType(Deg::Compiler::SG::RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

Deg::Compiler::SG::RecordType::RecordType(Deg::Compiler::SG::RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

Deg::Compiler::SG::EnumerationType::EnumerationType(Deg::Compiler::SG::EnumerationSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

Deg::Compiler::SG::ProgramType::ProgramType(Deg::Compiler::SG::ProgramSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

Deg::Compiler::SG::ProgramSymbol::ProgramSymbol()
	: ast_program(nullptr), Base(nullptr) {
	return;
}

Deg::Compiler::SG::RecordMemberSymbol::RecordMemberSymbol(std::unique_ptr<Type>& InputType)
	: InputType(std::move(InputType)), Offset(0) {
	return;
}

Deg::Compiler::SG::RecordSymbol::RecordSymbol()
	: ast_record(nullptr), QuantityMember(nullptr) {
	return;
}

Deg::Compiler::SG::FunctionArgumentSymbol::FunctionArgumentSymbol(std::unique_ptr<Type>& InputType)
	: InputType(std::move(InputType)), Offset(0) {
	return;
}

Deg::Compiler::SG::EnumerationMemberSymbol::EnumerationMemberSymbol(unsigned int Value)
	: Value(Value) {
	return;
}

SGVISITOR_ACCEPT_IMPL(NumberType);
SGVISITOR_ACCEPT_IMPL(BooleanType);
SGVISITOR_ACCEPT_IMPL(SetType);
SGVISITOR_ACCEPT_IMPL(ConstrainedSetType);
SGVISITOR_ACCEPT_IMPL(ProgramType);
SGVISITOR_ACCEPT_IMPL(RecordType);
SGVISITOR_ACCEPT_IMPL(FunctionType);
SGVISITOR_ACCEPT_IMPL(EnumerationType);
SGVISITOR_ACCEPT_IMPL(ErrorType);

SGVISITOR_ACCEPT_IMPL(NumberSymbol);
SGVISITOR_ACCEPT_IMPL(QuantitySymbol);
SGVISITOR_ACCEPT_IMPL(BooleanSymbol);
SGVISITOR_ACCEPT_IMPL(ProgramSymbol);
SGVISITOR_ACCEPT_IMPL(EnumerationMemberSymbol);
SGVISITOR_ACCEPT_IMPL(EnumerationSymbol);
SGVISITOR_ACCEPT_IMPL(FunctionArgumentSymbol);
SGVISITOR_ACCEPT_IMPL(FunctionSymbol);
SGVISITOR_ACCEPT_IMPL(RecordMemberSymbol);
SGVISITOR_ACCEPT_IMPL(RecordSymbol);
SGVISITOR_ACCEPT_IMPL(ErrorSymbol);
