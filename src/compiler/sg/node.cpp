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

Deg::Compiler::SG::Type::~Type() {
	return;
}

bool Deg::Compiler::SG::NumberType::CanAcceptValueOfType(const Type& t) const {
	const NumberType* nt = dynamic_cast<const NumberType*>(&t);
	return nt != nullptr;
}

bool Deg::Compiler::SG::BooleanType::CanAcceptValueOfType(const Type& t) const {
	const BooleanType* nt = dynamic_cast<const BooleanType*>(&t);
	return nt != nullptr;
}

bool Deg::Compiler::SG::SetType::CanAcceptValueOfType(const Type& t) const {
	const SetType* nt = dynamic_cast<const SetType*>(&t);
	if(nt) {
		return true;
	}

	const ConstrainedSetType* cnt = dynamic_cast<const ConstrainedSetType*>(&t);
	return nt != nullptr;
}

Deg::Compiler::SG::ConstrainedSetType::ConstrainedSetType(Deg::Compiler::SG::RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

bool Deg::Compiler::SG::ConstrainedSetType::CanAcceptValueOfType(const Type& t) const {
	const ConstrainedSetType* nt = dynamic_cast<const ConstrainedSetType*>(&t);
	if(nt) {
		return nt->ElementType == ElementType;
	}
	else {
		return false;
	}
}

Deg::Compiler::SG::ProgramType::ProgramType(Deg::Compiler::SG::ProgramSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

bool Deg::Compiler::SG::ProgramType::CanAcceptValueOfType(const Type& t) const {
	const ProgramType* nt = dynamic_cast<const ProgramType*>(&t);
	if(nt) {
		return nt->ElementType == ElementType;
	}
	else {
		return false;
	}
}

Deg::Compiler::SG::RecordType::RecordType(Deg::Compiler::SG::RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

bool Deg::Compiler::SG::RecordType::CanAcceptValueOfType(const Type& t) const {
	const RecordType* nt = dynamic_cast<const RecordType*>(&t);
	if(nt) {
		return nt->ElementType == ElementType;
	}
	else {
		return false;
	}
}

bool Deg::Compiler::SG::FunctionType::CanAcceptValueOfType(const Type& t) const {
	const FunctionType* nt = dynamic_cast<const FunctionType*>(&t);
	if(nt) {
		bool b = true;
		b &= ReturnType->CanAcceptValueOfType(*nt->ReturnType);
		b &= ArgumentTypes.size() == nt->ArgumentTypes.size();
		if(b) {
			for(size_t i = 0; i < ArgumentTypes.size(); ++i) {
				b &= ArgumentTypes[i]->CanAcceptValueOfType(*nt->ArgumentTypes[i]);
			}
		}
		return b;
	}
	else {
		return false;
	}
}

Deg::Compiler::SG::EnumerationType::EnumerationType(Deg::Compiler::SG::EnumerationSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

bool Deg::Compiler::SG::EnumerationType::CanAcceptValueOfType(const Type& t) const {
	const EnumerationType* nt = dynamic_cast<const EnumerationType*>(&t);
	if(nt) {
		return nt->ElementType == ElementType;
	}
	else {
		return false;
	}
}

bool Deg::Compiler::SG::ErrorType::CanAcceptValueOfType(const Type& t) const {
	return true;
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

Deg::Compiler::SG::NumericExpression::NumericExpression(Deg::Runtime::Math::DefaultFixed Value)
	: Value(Value) {
	return;
}

Deg::Compiler::SG::BooleanExpression::BooleanExpression(bool Value)
	: Value(Value) {
	return;
}

Deg::Compiler::SG::TypedSetExpression::TypedSetExpression(Deg::Compiler::SG::RecordSymbol* ElementType)
	: ElementType(ElementType) {
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

SGVISITOR_ACCEPT_IMPL(ErrorExpression);
SGVISITOR_ACCEPT_IMPL(NumericExpression);
SGVISITOR_ACCEPT_IMPL(BooleanExpression);
SGVISITOR_ACCEPT_IMPL(EmptySetExpression);
SGVISITOR_ACCEPT_IMPL(UniversalSetExpression);
SGVISITOR_ACCEPT_IMPL(TypedSetExpression);
