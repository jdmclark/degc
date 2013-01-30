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

Deg::Compiler::SG::RecordType::RecordType(Deg::Compiler::SG::RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

Deg::Compiler::SG::EnumerationType::EnumerationType(Deg::Compiler::SG::EnumerationSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

Deg::Compiler::SG::RecordMemberSymbol::RecordMemberSymbol(const std::string& Name, std::unique_ptr<Type>& InputType)
	: Name(Name), InputType(std::move(InputType)), Offset(0) {
	return;
}

Deg::Compiler::SG::RecordSymbol::RecordSymbol()
	: QuantityMember(nullptr) {
	return;
}

Deg::Compiler::SG::FunctionArgumentSymbol::FunctionArgumentSymbol(const std::string& Name, std::unique_ptr<Type>& InputType)
	: Name(Name), InputType(std::move(InputType)), Offset(0) {
	return;
}

Deg::Compiler::SG::EnumerationMemberSymbol::EnumerationMemberSymbol(const std::string& Name, unsigned int Value)
	: Name(Name), Value(Value) {
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
SGVISITOR_ACCEPT_IMPL(SetSymbol);
SGVISITOR_ACCEPT_IMPL(ProgramSymbol);
SGVISITOR_ACCEPT_IMPL(EnumerationMemberSymbol);
SGVISITOR_ACCEPT_IMPL(EnumerationSymbol);
SGVISITOR_ACCEPT_IMPL(FunctionArgumentSymbol);
SGVISITOR_ACCEPT_IMPL(FunctionSymbol);
SGVISITOR_ACCEPT_IMPL(RecordMemberSymbol);
SGVISITOR_ACCEPT_IMPL(RecordSymbol);
SGVISITOR_ACCEPT_IMPL(ErrorSymbol);
