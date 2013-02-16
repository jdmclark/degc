#include "factory.h"

using namespace Deg::Compiler::AST;

NamedTypename* Factory::MakeNamedTypename(const std::string& Value, const Diagnostics::ErrorLocation& yyl) {
	MAKE(NamedTypename);
	val->Value = Value;
	return val;
}

SetTypename* Factory::MakeSetTypename(const std::string& RecordType, const Diagnostics::ErrorLocation& yyl) {
	MAKE(SetTypename);
	val->RecordType = RecordType;
	return val;
}

FunctionTypename* Factory::MakeFunctionTypename(std::vector<Typename*>* DomainType, Typename* CodomainType, const Diagnostics::ErrorLocation& yyl) {
	MAKE(FunctionTypename);
	val->DomainType = DomainType;
	val->CodomainType = CodomainType;
	return val;
}

NumericLiteralExpression* Factory::MakeNumericLiteralExpression(const std::string& Value, const Diagnostics::ErrorLocation& yyl) {
	MAKE(NumericLiteralExpression);
	val->Value = Runtime::Math::DefaultFixed(Value);
	return val;
}

BooleanLiteralExpression* Factory::MakeBooleanLiteralExpression(bool Value, const Diagnostics::ErrorLocation& yyl) {
	MAKE(BooleanLiteralExpression);
	val->Value = Value;
	return val;
}

TypedSetExpression* Factory::MakeTypedSetExpression(const std::string& Typename, const Diagnostics::ErrorLocation& yyl) {
	MAKE(TypedSetExpression);
	val->Typename = Typename;
	return val;
}

ConstrainedSetExpression* Factory::MakeConstrainedSetExpression(const std::string& Typename, Expression* Constraint, const Diagnostics::ErrorLocation& yyl) {
	MAKE(ConstrainedSetExpression);
	val->Typename = Typename;
	val->Constraint = Constraint;
	return val;
}

PanicExpression* Factory::MakePanicExpression(const Diagnostics::ErrorLocation& yyl) {
	MAKE(PanicExpression);
	return val;
}

IdentifierExpression* Factory::MakeIdentifierExpression(const std::string& Identifier, const Diagnostics::ErrorLocation& yyl) {
	MAKE(IdentifierExpression);
	val->Identifier = Identifier;
	return val;
}

FunctionCallExpression* Factory::MakeFunctionCallExpression(Expression* Target, std::vector<Expression*>* Arguments, const Diagnostics::ErrorLocation& yyl) {
	MAKE(FunctionCallExpression);
	val->Target = Target;
	val->Arguments = Arguments;
	return val;
}

MemberAccessExpression* Factory::MakeMemberAccessExpression(Expression* Target, const std::string& MemberName, const Diagnostics::ErrorLocation& yyl) {
	MAKE(MemberAccessExpression);
	val->Target = Target;
	val->MemberName = MemberName;
	return val;
}

UnaryExpression* Factory::MakeUnaryExpression(UnaryOperator Operator, Expression* Value, const Diagnostics::ErrorLocation& yyl) {
	MAKE(UnaryExpression);
	val->Operator = Operator;
	val->Value = Value;
	return val;
}

ExistsExpression* Factory::MakeExistsExpression(Expression* Value, const Diagnostics::ErrorLocation& yyl) {
	MAKE(ExistsExpression);
	val->Value = Value;
	return val;
}

InfixExpression* Factory::MakeInfixExpression(InfixOperator Operator, Expression* LeftValue, Expression* RightValue, const Diagnostics::ErrorLocation& yyl) {
	MAKE(InfixExpression);
	val->Operator = Operator;
	val->LeftValue = LeftValue;
	val->RightValue = RightValue;
	return val;
}

FunctionIfElseExpression* Factory::MakeFunctionIfElseExpression(Expression* Predicate, Expression* Code, Expression* ElseCode, const Diagnostics::ErrorLocation& yyl) {
	MAKE(FunctionIfElseExpression);
	val->Predicate = Predicate;
	val->Code = Code;
	val->ElseCode = ElseCode;
	return val;
}

NamedStatement* Factory::MakeNamedStatement(const std::string& Name, Statement* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(NamedStatement);
	val->Name = Name;
	val->Code = Code;
	return val;
}

CompoundStatement* Factory::MakeCompoundStatement(std::vector<Statement*>* Statements, const Diagnostics::ErrorLocation& yyl) {
	MAKE(CompoundStatement);
	val->Statements = Statements;
	return val;
}

AssertStatement* Factory::MakeAssertStatement(Expression* Value, const Diagnostics::ErrorLocation& yyl) {
	MAKE(AssertStatement);
	val->Value = Value;
	return val;
}

EmbedStatement* Factory::MakeEmbedStatement(Expression* Value, const Diagnostics::ErrorLocation& yyl) {
	MAKE(EmbedStatement);
	val->Value = Value;
	return val;
}

EmbedInlineStatement* Factory::MakeEmbedInlineStatement(Statement* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(EmbedInlineStatement);
	val->Code = Code;
	return val;
}

DisjunctionStatement* Factory::MakeDisjunctionStatement(std::vector<Statement*>* Statements, const Diagnostics::ErrorLocation& yyl) {
	MAKE(DisjunctionStatement);
	val->Statements = Statements;
	return val;
}

ForAllStatement* Factory::MakeForAllStatement(const std::string& Typename, const std::string& ElementName, Expression* Set, Statement* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(ForAllStatement);
	val->Typename = Typename;
	val->ElementName = ElementName;
	val->Set = Set;
	val->Code = Code;
	return val;
}

ForAnyStatement* Factory::MakeForAnyStatement(const std::string& Typename, const std::string& ElementName, Expression* Set, Statement* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(ForAnyStatement);
	val->Typename = Typename;
	val->ElementName = ElementName;
	val->Set = Set;
	val->Code = Code;
	return val;
}

ForBestStatement* Factory::MakeForBestStatement(const std::string& Typename, const std::string& ElementName, Expression* Predicate, Expression* Set, Statement* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(ForBestStatement);
	val->Typename = Typename;
	val->ElementName = ElementName;
	val->Predicate = Predicate;
	val->Set = Set;
	val->Code = Code;
	return val;
}

IfStatement* Factory::MakeIfStatement(Expression* Predicate, Statement* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(IfStatement);
	val->Predicate = Predicate;
	val->Code = Code;
	return val;
}

IfElseStatement* Factory::MakeIfElseStatement(Expression* Predicate, Statement* Code, Statement* ElseCode, const Diagnostics::ErrorLocation& yyl) {
	MAKE(IfElseStatement);
	val->Predicate = Predicate;
	val->Code = Code;
	val->ElseCode = ElseCode;
	return val;
}

TakeStatement* Factory::MakeTakeStatement(Expression* Amount, Expression* Set, const Diagnostics::ErrorLocation& yyl) {
	MAKE(TakeStatement);
	val->Amount = Amount;
	val->Set = Set;
	return val;
}

LimitStatement* Factory::MakeLimitStatement(Expression* Amount, Expression* Set, const Diagnostics::ErrorLocation& yyl) {
	MAKE(LimitStatement);
	val->Amount = Amount;
	val->Set = Set;
	return val;
}

Program* Factory::MakeProgram(const std::string& Name, const std::string& Extends, Statement* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(Program);
	val->Name = Name;
	val->Extends = Extends;
	val->Code = Code;
	return val;
}

FunctionArgument* Factory::MakeFunctionArgument(Typename* Type, const std::string& Name, const Diagnostics::ErrorLocation& yyl) {
	MAKE(FunctionArgument);
	val->Type = Type;
	val->Name = Name;
	return val;
}

Function* Factory::MakeFunction(const std::string& Name, std::vector<FunctionArgument*>* Arguments, Typename* Codomain, Expression* Code, const Diagnostics::ErrorLocation& yyl) {
	MAKE(Function);
	val->Name = Name;
	val->Arguments = Arguments;
	val->Codomain = Codomain;
	val->Code = Code;
	return val;
}

RecordMember* Factory::MakeRecordMember(Typename* Type, const std::string& Name, const Diagnostics::ErrorLocation& yyl) {
	MAKE(RecordMember);
	val->Type = Type;
	val->Name = Name;
	return val;
}

Record* Factory::MakeRecord(const std::string& Name, std::vector<RecordMember*>* Members, const Diagnostics::ErrorLocation& yyl) {
	MAKE(Record);
	val->Name = Name;
	val->Members = Members;
	return val;
}

EnumerationMember* Factory::MakeEnumerationMember(const std::string& Name, const Diagnostics::ErrorLocation& yyl) {
	MAKE(EnumerationMember);
	val->Name = Name;
	return val;
}

Enumeration* Factory::MakeEnumeration(const std::string& Name, std::vector<EnumerationMember*>* Members, const Diagnostics::ErrorLocation& yyl) {
	MAKE(Enumeration);
	val->Name = Name;
	val->Members = Members;
	return val;
}

ModuleName* Factory::MakeModuleName(const std::string& Name, const Diagnostics::ErrorLocation& yyl) {
	MAKE(ModuleName);
	val->Name = Name;
	return val;
}

SymbolImport* Factory::MakeSymbolImport(const std::string& OriginalName, const std::string& ImportName, const Diagnostics::ErrorLocation& yyl) {
	MAKE(SymbolImport);
	val->OriginalName = OriginalName;
	val->ImportName = ImportName;
	return val;
}

ModuleImport* Factory::MakeModuleImport(ModuleName* Name, std::vector<SymbolImport*>* Symbols, const Diagnostics::ErrorLocation& yyl) {
	MAKE(ModuleImport);
	val->Name = Name;
	val->Symbols = Symbols;
	return val;
}

TranslationUnit* Factory::MakeTranslationUnit(ModuleName* Name, std::vector<ModuleImport*>* Imports, std::vector<Declaration*>* Declarations, const Diagnostics::ErrorLocation& yyl) {
	MAKE(TranslationUnit);
	val->Name = Name;
	val->Imports = Imports;
	val->Declarations = Declarations;
	return val;
}
