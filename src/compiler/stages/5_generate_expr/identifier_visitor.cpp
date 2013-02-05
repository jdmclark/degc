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
