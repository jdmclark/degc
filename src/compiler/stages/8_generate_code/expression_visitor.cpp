#include "expression_visitor.h"
#include "identifier_visitor.h"
#include "function_visitor.h"
#include "set_expression_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::GenerateCode::ExpressionVisitor;

ExpressionVisitor::ExpressionVisitor(IR::Printer& code, Runtime::Code::RecordTypeTable& recordTypeTable, Diagnostics::Report& report)
	: SG::Visitor("GenerateCode::ExpressionVisitor", report), code(code), recordTypeTable(recordTypeTable) {
	return;
}

void ExpressionVisitor::VisitNumericExpression(NumericExpression& e) {
	code.ConstN(e.Value);
}

void ExpressionVisitor::VisitBooleanExpression(BooleanExpression& e) {
	code.ConstB(e.Value);
}

void ExpressionVisitor::VisitTypedSetExpression(TypedSetExpression& e) {
	code.Cons(e.ElementType->Members.children_size());
}

void ExpressionVisitor::VisitConstrainedSetExpression(ConstrainedSetExpression& e) {
	SetExpressionVisitor v(code, recordTypeTable, e.ElementType->Members.children_size(), Report);
	e.Filter->Accept(v);
}

void ExpressionVisitor::VisitPanicExpression(PanicExpression& e) {
	code.Panic();
}

void ExpressionVisitor::VisitIdentifierExpression(IdentifierExpression& e) {
	IdentifierVisitor v(code, Report);
	e.ReferencedNode->Accept(v);
}

void ExpressionVisitor::VisitFunctionCallExpression(FunctionCallExpression& e) {
	// Generate code for arguments.
	for(auto& arg : e.ArgumentExpressions) {
		arg->Accept(*this);
	}

	FunctionVisitor fv(e.ArgumentExpressions.size(), code, Report);
	e.FunctionTargetExpression->Accept(fv);
}

void ExpressionVisitor::VisitMemberAccessExpression(MemberAccessExpression& e) {
	e.Base->Accept(*this);

	SG::BooleanType* bt = dynamic_cast<SG::BooleanType*>(e.Member->InputType.get());
	SG::NumberType* nt = dynamic_cast<SG::NumberType*>(e.Member->InputType.get());
	SG::EnumerationType* et = dynamic_cast<SG::EnumerationType*>(e.Member->InputType.get());

	if(bt) {
		code.MemB(e.Member->Index);
	}
	else if(nt) {
		code.MemN(e.Member->Index);
	}
	else if(et) {
		code.MemN(e.Member->Index);
	}
	else {
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
				"GenerateCode::ExpressionVisitor", "record member type"));
	}
}

void ExpressionVisitor::VisitUnaryExpression(UnaryExpression& e) {
	e.Value->Accept(*this);
	switch(e.Operator) {
	case AST::UnaryOperator::Minus:
		code.Neg();
		break;

	case AST::UnaryOperator::Not:
		code.LNot();
		break;

	default:
		DefaultAction("VisitUnaryExpression::unary operator", e);
		break;
	}
}

void ExpressionVisitor::VisitExistsExpression(ExistsExpression& e) {
	e.Value->Accept(*this);
	code.Exists(recordTypeTable.GetRecordType(e.ElementType->UniversalUniqueName).type_id);
}

void ExpressionVisitor::VisitInfixExpression(InfixExpression& e) {
	e.LeftValue->Accept(*this);
	e.RightValue->Accept(*this);
	switch(e.Operator) {
	case AST::InfixOperator::Addition:
		code.Add();
		break;

	case AST::InfixOperator::Subtraction:
		code.Sub();
		break;

	case AST::InfixOperator::Multiplication:
		code.Mul();
		break;

	case AST::InfixOperator::Division:
		code.Div();
		break;

	case AST::InfixOperator::Greater:
		code.CGt();
		break;

	case AST::InfixOperator::GreaterEqual:
		code.CGeq();
		break;

	case AST::InfixOperator::Less:
		code.CLt();
		break;

	case AST::InfixOperator::LessEqual:
		code.CLeq();
		break;

	case AST::InfixOperator::Equal:
		code.CEq();
		break;

	case AST::InfixOperator::NotEqual:
		code.CNeq();
		break;

	case AST::InfixOperator::And:
		code.LAnd();
		break;

	case AST::InfixOperator::Or:
		code.LOr();
		break;

	default:
		DefaultAction("VisitInfixExpression::infix operator", e);
		break;
	}
}

void ExpressionVisitor::VisitFunctionIfElseExpression(FunctionIfElseExpression& e) {
	std::string t_label = code.GenerateUniqueLabel();
	std::string f_label = code.GenerateUniqueLabel();
	std::string exit_label = code.GenerateUniqueLabel();

	e.Predicate->Accept(*this);
	code.Btf(t_label, f_label);

	code.Label(t_label);
	e.IfCode->Accept(*this);
	code.Jmp(exit_label);

	code.Label(f_label);
	e.ElseCode->Accept(*this);

	code.Label(exit_label);
}
