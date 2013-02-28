#include "node.h"
#include "visitor.h"

using namespace Deg::Compiler::SG;

Node::~Node() {
	return;
}

#define SGVISITOR_ACCEPT_IMPL(x)						\
	void Deg::Compiler::SG::x::Accept(Visitor& v) {		\
		v.Visit##x(*this);								\
		return;											\
	}

Type::~Type() {
	return;
}

std::unique_ptr<Type> NumberType::Clone() const {
	return std::unique_ptr<Type>(new NumberType());
}

bool NumberType::CanAcceptValueOfType(const Type& t) const {
	const ErrorType* et = dynamic_cast<const ErrorType*>(&t);
	const NumberType* nt = dynamic_cast<const NumberType*>(&t);
	return et != nullptr || nt != nullptr;
}

std::unique_ptr<Type> BooleanType::Clone() const {
	return std::unique_ptr<Type>(new BooleanType());
}

bool BooleanType::CanAcceptValueOfType(const Type& t) const {
	const ErrorType* et = dynamic_cast<const ErrorType*>(&t);
	const BooleanType* nt = dynamic_cast<const BooleanType*>(&t);
	return et != nullptr || nt != nullptr;
}

SetType::SetType(RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

std::unique_ptr<Type> SetType::Clone() const {
	return std::unique_ptr<Type>(new SetType(ElementType));
}

bool SetType::CanAcceptValueOfType(const Type& t) const {
	const SetType* nt = dynamic_cast<const SetType*>(&t);
	if(nt) {
		return nt->ElementType == ElementType;
	}
	else {
		const ErrorType* et = dynamic_cast<const ErrorType*>(&t);
		return et != nullptr;
	}
}

ProgramType::ProgramType(ProgramSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

std::unique_ptr<Type> ProgramType::Clone() const {
	return std::unique_ptr<Type>(new ProgramType(ElementType));
}

bool ProgramType::CanAcceptValueOfType(const Type& t) const {
	const ProgramType* nt = dynamic_cast<const ProgramType*>(&t);
	if(nt) {
		return nt->ElementType->InheritsFrom(*ElementType);
	}
	else {
		const ErrorType* et = dynamic_cast<const ErrorType*>(&t);
		return et != nullptr;
	}
}

RecordType::RecordType(RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

std::unique_ptr<Type> RecordType::Clone() const {
	return std::unique_ptr<Type>(new RecordType(ElementType));
}

bool RecordType::CanAcceptValueOfType(const Type& t) const {
	const RecordType* nt = dynamic_cast<const RecordType*>(&t);
	if(nt) {
		return nt->ElementType == ElementType;
	}
	else {
		const ErrorType* et = dynamic_cast<const ErrorType*>(&t);
		return et != nullptr;
	}
}

std::unique_ptr<Type> FunctionType::Clone() const {
	std::unique_ptr<FunctionType> ft(new FunctionType());
	ft->ReturnType = ReturnType->Clone();
	for(const auto& arg : ArgumentTypes) {
		ft->ArgumentTypes.push_back(arg->Clone());
	}
	return std::move(ft);
}

bool FunctionType::CanAcceptValueOfType(const Type& t) const {
	const FunctionType* nt = dynamic_cast<const FunctionType*>(&t);
	if(nt) {
		bool b = true;
		b &= ReturnType->CanAcceptValueOfType(*nt->ReturnType);
		b &= ArgumentTypes.size() == nt->ArgumentTypes.size();
		if(b) {
			for(size_t i = 0; i < ArgumentTypes.size(); ++i) {
				b &= nt->ArgumentTypes[i]->CanAcceptValueOfType(*ArgumentTypes[i]);
			}
		}
		return b;
	}
	else {
		const ErrorType* et = dynamic_cast<const ErrorType*>(&t);
		return et != nullptr;
	}
}

EnumerationType::EnumerationType(EnumerationSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

std::unique_ptr<Type> EnumerationType::Clone() const {
	return std::unique_ptr<Type>(new EnumerationType(ElementType));
}

bool EnumerationType::CanAcceptValueOfType(const Type& t) const {
	const EnumerationType* nt = dynamic_cast<const EnumerationType*>(&t);
	if(nt) {
		return nt->ElementType == ElementType;
	}
	else {
		const ErrorType* et = dynamic_cast<const ErrorType*>(&t);
		return et != nullptr;
	}
}

std::unique_ptr<Type> ErrorType::Clone() const {
	return std::unique_ptr<Type>(new ErrorType());
}

bool ErrorType::CanAcceptValueOfType(const Type& t) const {
	return true;
}

ProgramSymbol::ProgramSymbol()
	: ast_program(nullptr), Base(nullptr) {
	return;
}

bool ProgramSymbol::InheritsFrom(const ProgramSymbol& p) const {
	if(this == &p) {
		return true;
	}
	else if(Base) {
		return Base->InheritsFrom(p);
	}
	else {
		return false;
	}
}

RecordMemberSymbol::RecordMemberSymbol(std::unique_ptr<Type>& InputType)
	: InputType(std::move(InputType)), Offset(0) {
	return;
}

RecordSymbol::RecordSymbol()
	: ast_record(nullptr), QuantityMember(nullptr) {
	return;
}

FunctionArgumentSymbol::FunctionArgumentSymbol(std::unique_ptr<Type>& InputType, unsigned int Index)
	: InputType(std::move(InputType)), Index(Index), Offset(0) {
	return;
}

EnumerationMemberSymbol::EnumerationMemberSymbol(unsigned int Value)
	: Value(Value) {
	return;
}

VariableSymbol::VariableSymbol(RecordSymbol* ElementType)
	: ElementType(ElementType) {
	return;
}

NumericExpression::NumericExpression(Deg::Runtime::Math::DefaultFixed Value)
	: Value(Value) {
	return;
}

BooleanExpression::BooleanExpression(bool Value)
	: Value(Value) {
	return;
}

TypedSetExpression::TypedSetExpression(Deg::Compiler::SG::RecordSymbol* ElementType, const Diagnostics::ErrorLocation& Location)
	: ElementType(ElementType), Location(Location) {
	return;
}

ConstrainedSetExpression::ConstrainedSetExpression(RecordSymbol* ElementType, std::unique_ptr<Expression>& Filter, const Diagnostics::ErrorLocation& Location)
	: ElementType(ElementType), Filter(std::move(Filter)), Location(Location) {
	return;
}

IdentifierExpression::IdentifierExpression(Node* ReferencedNode)
	: ReferencedNode(ReferencedNode) {
	return;
}

MemberAccessExpression::MemberAccessExpression(std::unique_ptr<Expression>& Base, RecordMemberSymbol* Member)
	: Base(std::move(Base)), Member(Member) {
	return;
}

UnaryExpression::UnaryExpression(std::unique_ptr<Expression>& Value, AST::UnaryOperator Operator)
	: Value(std::move(Value)), Operator(Operator) {
	return;
}

ExistsExpression::ExistsExpression(std::unique_ptr<Expression>& Value)
	: Value(std::move(Value)) {
	return;
}

InfixExpression::InfixExpression(std::unique_ptr<Expression>& LeftValue, std::unique_ptr<Expression>& RightValue, AST::InfixOperator Operator)
	: LeftValue(std::move(LeftValue)), RightValue(std::move(RightValue)), Operator(Operator) {
	return;
}

FunctionIfElseExpression::FunctionIfElseExpression(std::unique_ptr<Expression>& Predicate, std::unique_ptr<Expression>& IfCode, std::unique_ptr<Expression>& ElseCode)
	: Predicate(std::move(Predicate)), IfCode(std::move(IfCode)), ElseCode(std::move(ElseCode)) {
	return;
}

SetClauseExpression::SetClauseExpression(RecordMemberSymbol* Member, AST::InfixOperator Operator, std::unique_ptr<Expression>& Value)
	: Member(Member), Operator(Operator), Value(std::move(Value)) {
	return;
}

AssertStatement::AssertStatement(std::unique_ptr<Expression>& Value)
	: Value(std::move(Value)) {
	return;
}

EmbedStatement::EmbedStatement(std::unique_ptr<Expression>& Value)
	: Value(std::move(Value)) {
	return;
}

IfStatement::IfStatement(std::unique_ptr<Expression>& Predicate, std::unique_ptr<Statement>& Code)
	: Predicate(std::move(Predicate)), Code(std::move(Code)) {
	return;
}

IfElseStatement::IfElseStatement(std::unique_ptr<Expression>& Predicate, std::unique_ptr<Statement>& Code, std::unique_ptr<Statement>& ElseCode)
	: Predicate(std::move(Predicate)), Code(std::move(Code)), ElseCode(std::move(ElseCode)) {
	return;
}

TakeStatement::TakeStatement(AST::TakeStatement* ast_node, std::unique_ptr<Expression>& Amount, std::unique_ptr<Expression>& Set)
	: ast_node(ast_node), Amount(std::move(Amount)), Set(std::move(Set)) {
	return;
}

LimitStatement::LimitStatement(AST::LimitStatement* ast_node, std::unique_ptr<Expression>& Amount, std::unique_ptr<Expression>& Set)
	: ast_node(ast_node), Amount(std::move(Amount)), Set(std::move(Set)) {
	return;
}

SGVISITOR_ACCEPT_IMPL(NumberType);
SGVISITOR_ACCEPT_IMPL(BooleanType);
SGVISITOR_ACCEPT_IMPL(SetType);
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
SGVISITOR_ACCEPT_IMPL(VariableSymbol);
SGVISITOR_ACCEPT_IMPL(ErrorSymbol);

SGVISITOR_ACCEPT_IMPL(ErrorExpression);
SGVISITOR_ACCEPT_IMPL(NumericExpression);
SGVISITOR_ACCEPT_IMPL(BooleanExpression);
SGVISITOR_ACCEPT_IMPL(TypedSetExpression);
SGVISITOR_ACCEPT_IMPL(ConstrainedSetExpression);
SGVISITOR_ACCEPT_IMPL(PanicExpression);
SGVISITOR_ACCEPT_IMPL(IdentifierExpression);
SGVISITOR_ACCEPT_IMPL(FunctionCallExpression);
SGVISITOR_ACCEPT_IMPL(MemberAccessExpression);
SGVISITOR_ACCEPT_IMPL(UnaryExpression);
SGVISITOR_ACCEPT_IMPL(ExistsExpression);
SGVISITOR_ACCEPT_IMPL(InfixExpression);
SGVISITOR_ACCEPT_IMPL(FunctionIfElseExpression);
SGVISITOR_ACCEPT_IMPL(SetClauseExpression);

SGVISITOR_ACCEPT_IMPL(CompoundStatement);
SGVISITOR_ACCEPT_IMPL(AssertStatement);
SGVISITOR_ACCEPT_IMPL(EmbedStatement);
SGVISITOR_ACCEPT_IMPL(DisjunctionStatement);
SGVISITOR_ACCEPT_IMPL(ForAllStatement);
SGVISITOR_ACCEPT_IMPL(ForAnyStatement);
SGVISITOR_ACCEPT_IMPL(ForBestStatement);
SGVISITOR_ACCEPT_IMPL(IfStatement);
SGVISITOR_ACCEPT_IMPL(IfElseStatement);
SGVISITOR_ACCEPT_IMPL(TakeStatement);
SGVISITOR_ACCEPT_IMPL(LimitStatement);
SGVISITOR_ACCEPT_IMPL(ErrorStatement);
