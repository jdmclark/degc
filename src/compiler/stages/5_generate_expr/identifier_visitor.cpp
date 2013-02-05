#include "identifier_visitor.h"
#include "compiler/sg/error_helper.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateExpressions::IdentifierVisitor;

IdentifierVisitor::IdentifierVisitor(ScopeStack& scope, const std::string& symbolname, const Diagnostics::ErrorLocation& location, Diagnostics::Report& report)
	: SG::Visitor("GenerateExpressions::IdentifierVisitor", report), scope(scope), symbolname(symbolname), location(location), IsConstantValue(false) {
	return;
}

void IdentifierVisitor::DefaultAction(const std::string& action, Node& n) {
	ErrorHelper::SymbolNotExpression(Report, VisitorName, location, symbolname);
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<Expression>(new ErrorExpression());
	GeneratedExpressionType = std::unique_ptr<Type>(new ErrorType());
}

void IdentifierVisitor::VisitProgramSymbol(SG::ProgramSymbol& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	GeneratedExpressionType = std::unique_ptr<Type>(new SG::ProgramType(&n));
}

void IdentifierVisitor::VisitRecordMemberSymbol(SG::RecordMemberSymbol& n) {
	IsConstantValue = false;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	GeneratedExpressionType = n.InputType->Clone();
}

void IdentifierVisitor::VisitFunctionArgumentSymbol(SG::FunctionArgumentSymbol& n) {
	IsConstantValue = false;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	GeneratedExpressionType = n.InputType->Clone();
}

void IdentifierVisitor::VisitFunctionSymbol(SG::FunctionSymbol& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));

	std::unique_ptr<SG::FunctionType> fn_type(new SG::FunctionType());
	fn_type->ReturnType = n.CodomainType->Clone();
	for(auto& arg : n.Arguments) {
		SG::FunctionArgumentSymbol* arg_symbol = dynamic_cast<SG::FunctionArgumentSymbol*>(arg.get());
		if(arg_symbol) {
			fn_type->ArgumentTypes.push_back(arg_symbol->InputType->Clone());
		}
		else {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
					VisitorName, "unimplemented function argument type"));
			GeneratedExpression = std::unique_ptr<Expression>(new SG::ErrorExpression());
			GeneratedExpressionType = std::unique_ptr<Type>(new SG::ErrorType());
			return;
		}
	}

	GeneratedExpressionType = std::move(fn_type);
}
