#include "expression_visitor.h"
#include "compiler/sg/error_helper.h"
#include "identifier_visitor.h"
#include <boost/format.hpp>

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateExpressions::ExpressionVisitor;

ExpressionVisitor::ExpressionVisitor(SG::ScopeStack& scope, Diagnostics::Report& report)
	: AST::Visitor("GenerateExpressions::ExpressionVisitor", report), scope(scope) {
	return;
}

void ExpressionVisitor::GenerateErrorResult() {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::ErrorType());
}

void ExpressionVisitor::VisitNumericLiteralExpression(AST::NumericLiteralExpression& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(n.Value));
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::NumberType());
}

void ExpressionVisitor::VisitBooleanLiteralExpression(AST::BooleanLiteralExpression& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(n.Value));
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
}

void ExpressionVisitor::VisitTypedSetExpression(AST::TypedSetExpression& n) {
	try {
		SG::Node& element_node = scope.GetSymbol(n.Typename);
		SG::RecordSymbol* rec_node = dynamic_cast<SG::RecordSymbol*>(&element_node);
		if(rec_node) {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::TypedSetExpression(rec_node, n.Location));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::SetType(rec_node));
		}
		else {
			GenerateErrorResult();
			SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
		}
	}
	catch(...) {
		GenerateErrorResult();
		SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
	}
}

void ExpressionVisitor::VisitConstrainedSetExpression(AST::ConstrainedSetExpression& n) {
	try {
		SG::Node& element_node = scope.GetSymbol(n.Typename);
		SG::RecordSymbol* rec_node = dynamic_cast<SG::RecordSymbol*>(&element_node);
		if(rec_node) {
			// Check inner expression for boolean type.
			scope.PushScope(rec_node->Members);
			ExpressionVisitor filter_v(scope, Report);
			n.Constraint->Accept(filter_v);
			scope.PopScope();

			std::unique_ptr<SG::Expression> filter;

			SG::BooleanType t;
			if(t.CanAcceptValueOfType(*filter_v.GeneratedExpressionType)) {
				filter = std::move(filter_v.GeneratedExpression);
			}
			else {
				SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.Location);
				filter = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
			}

			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ConstrainedSetExpression(rec_node, filter, n.Location));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::SetType(rec_node));
		}
		else {
			GenerateErrorResult();
			SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
		}
	}
	catch(...) {
		GenerateErrorResult();
		SG::ErrorHelper::RecordNameExpected(Report, VisitorName, n.Location, n.Typename);
	}
}

void ExpressionVisitor::VisitPanicExpression(AST::PanicExpression& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::PanicExpression());
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::ErrorType());
}

void ExpressionVisitor::VisitIdentifierExpression(AST::IdentifierExpression& n) {
	try {
		SG::Node& node = scope.GetSymbol(n.Identifier);
		IdentifierVisitor v(n.Identifier, n.Location, Report);
		node.Accept(v);
		GeneratedExpression = std::move(v.GeneratedExpression);
		GeneratedExpressionType = std::move(v.GeneratedExpressionType);
	}
	catch(...) {
		SG::ErrorHelper::UndefinedSymbol(Report, VisitorName, n.Location, n.Identifier);
		GenerateErrorResult();
	}
}

void ExpressionVisitor::VisitFunctionCallExpression(AST::FunctionCallExpression& n) {
	ExpressionVisitor fv(scope, Report);
	n.Target->Accept(fv);

	SG::ErrorType* error_type = dynamic_cast<SG::ErrorType*>(fv.GeneratedExpressionType.get());
	if(error_type) {
		// Already an error.
		// Silently consume and check arguments as much as possible.
		for(auto arg : *n.Arguments) {
			ExpressionVisitor av(scope, Report);
			arg->Accept(av);
		}

		GenerateErrorResult();
		return;
	}

	SG::FunctionType* fn_type = dynamic_cast<SG::FunctionType*>(fv.GeneratedExpressionType.get());
	if(fn_type) {
		std::unique_ptr<SG::FunctionCallExpression> fexp(new SG::FunctionCallExpression());
		fexp->FunctionTargetExpression = std::move(fv.GeneratedExpression);

		if(fn_type->ArgumentTypes.size() != n.Arguments->size()) {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FunctionArgCountMismatch, Diagnostics::ErrorLevel::Error,
					VisitorName, boost::str(boost::format("function expects %d arguments, was passed %d") % fn_type->ArgumentTypes.size() % n.Arguments->size()),
					n.Location));
			GenerateErrorResult();
			return;
		}

		for(size_t i = 0; i < n.Arguments->size(); ++i) {
			ExpressionVisitor av(scope, Report);
			(*n.Arguments)[i]->Accept(av);
			if(fn_type->ArgumentTypes[i]->CanAcceptValueOfType(*av.GeneratedExpressionType)) {
				fexp->ArgumentExpressions.push_back(std::move(av.GeneratedExpression));
			}
			else {
				Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FunctionArgTypeMismatch, Diagnostics::ErrorLevel::Error,
						VisitorName, boost::str(boost::format("argument %d type mismatch") % (i + 1)), (*n.Arguments)[i]->Location));
				fexp->ArgumentExpressions.push_back(std::unique_ptr<SG::Expression>(new SG::ErrorExpression()));
			}
		}

		GeneratedExpression = std::move(fexp);
		GeneratedExpressionType = fn_type->ReturnType->Clone();

		return;
	}

	GenerateErrorResult();
	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FunctionCallToNonFunction, Diagnostics::ErrorLevel::Error,
			VisitorName, "attempted function call to non-function value", n.Location));
}

void ExpressionVisitor::VisitMemberAccessExpression(AST::MemberAccessExpression& n) {
	AST::IdentifierExpression* id_expr = dynamic_cast<IdentifierExpression*>(n.Target);
	if(id_expr) {
		// Test for special case - enumerations.
		try {
			SG::EnumerationSymbol& enum_node = dynamic_cast<SG::EnumerationSymbol&>(scope.GetSymbol(id_expr->Identifier));
			if(enum_node.Values.IsMember(n.MemberName)) {
				GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&enum_node.Values.GetMember(n.MemberName)));
				GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::EnumerationType(&enum_node));
				return;
			}
			else {
				Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::EnumMissingMember, Diagnostics::ErrorLevel::Error,
						VisitorName, boost::str(boost::format("enumeration \'%s\' does not have value \'%s\'") % id_expr->Identifier % n.MemberName),
						n.Location));
				GenerateErrorResult();
				return;
			}
		}
		catch(...) {
			// Not an enumeration.
			// Silently consume error and continue.
		}
	}

	ExpressionVisitor v(scope, Report);
	n.Target->Accept(v);

	SG::ErrorExpression* err_exp = dynamic_cast<SG::ErrorExpression*>(v.GeneratedExpression.get());
	if(err_exp) {
		// Expression is already an error type.
		// Silently consume error.
		GenerateErrorResult();
		return;
	}

	SG::RecordType* rt = dynamic_cast<SG::RecordType*>(v.GeneratedExpressionType.get());
	if(rt) {
		try {
			if(rt->ElementType->Members.IsMember(n.MemberName)) {
				SG::RecordMemberSymbol& rec_member = dynamic_cast<SG::RecordMemberSymbol&>(rt->ElementType->Members.GetMember(n.MemberName));
				GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::MemberAccessExpression(v.GeneratedExpression, &rec_member));
				GeneratedExpressionType = rec_member.InputType->Clone();
				return;
			}
			else {
				GenerateErrorResult();
				Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::RecordMissingMember, Diagnostics::ErrorLevel::Error,
						VisitorName, boost::str(boost::format("record value does not have member \'%s\'") % n.MemberName), n.Location));
				return;
			}
		}
		catch(...) {
			GenerateErrorResult();
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
					VisitorName, "unimplemented record member type", n.Location));
			return;
		}
	}

	GenerateErrorResult();
	SG::ErrorHelper::TypeDoesNotHaveMembers(Report, VisitorName, n.Location);
}

void ExpressionVisitor::VisitUnaryExpression(AST::UnaryExpression& n) {
	ExpressionVisitor val_v(scope, Report);
	n.Value->Accept(val_v);

	SG::ErrorType* et = dynamic_cast<SG::ErrorType*>(val_v.GeneratedExpressionType.get());
	if(et) {
		// Generated value is already an error. Consume silently.
		GenerateErrorResult();
		return;
	}

	switch(n.Operator) {
	case UnaryOperator::Minus: {
		SG::NumberType* nt = dynamic_cast<SG::NumberType*>(val_v.GeneratedExpressionType.get());
		if(nt) {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::UnaryExpression(val_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::NumberType());
		}
		else {
			GenerateErrorResult();
			SG::ErrorHelper::NumberExpected(Report, VisitorName, n.Value->Location);
		}
	}
	break;

	case UnaryOperator::Not: {
		SG::BooleanType* bt = dynamic_cast<SG::BooleanType*>(val_v.GeneratedExpressionType.get());
		if(bt) {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::UnaryExpression(val_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
		}
		else {
			GenerateErrorResult();
			SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.Value->Location);
		}
	}
	break;

	default:
		GenerateErrorResult();
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
				VisitorName, "unimplemented unary operator", n.Location));
		break;
	}
}

void ExpressionVisitor::VisitExistsExpression(AST::ExistsExpression& n) {
	ExpressionVisitor value_v(scope, Report);
	n.Value->Accept(value_v);

	SG::ErrorType* vt = dynamic_cast<SG::ErrorType*>(value_v.GeneratedExpressionType.get());
	if(vt != nullptr) {
		// Generated value is already an error. Consume silently.
		GenerateErrorResult();
		return;
	}

	SG::SetType* st = dynamic_cast<SG::SetType*>(value_v.GeneratedExpressionType.get());
	if(st != nullptr) {
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ExistsExpression(value_v.GeneratedExpression));
		GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
		return;
	}

	GenerateErrorResult();
	SG::ErrorHelper::SetExpected(Report, VisitorName, n.Value->Location);
}

void ExpressionVisitor::VisitInfixExpression(AST::InfixExpression& n) {
	ExpressionVisitor left_v(scope, Report);
	n.LeftValue->Accept(left_v);

	ExpressionVisitor right_v(scope, Report);
	n.RightValue->Accept(right_v);

	SG::ErrorType* let = dynamic_cast<SG::ErrorType*>(left_v.GeneratedExpressionType.get());
	SG::ErrorType* ret = dynamic_cast<SG::ErrorType*>(right_v.GeneratedExpressionType.get());
	if(let != nullptr || ret != nullptr) {
		// Generated value is already an error. Consume silently.
		GenerateErrorResult();
		return;
	}

	switch(n.Operator) {
	case InfixOperator::Addition:
	case InfixOperator::Subtraction:
	case InfixOperator::Multiplication:
	case InfixOperator::Division: {
		SG::NumberType* lnt = dynamic_cast<SG::NumberType*>(left_v.GeneratedExpressionType.get());
		SG::NumberType* rnt = dynamic_cast<SG::NumberType*>(right_v.GeneratedExpressionType.get());
		if(lnt == nullptr || rnt == nullptr) {
			if(lnt == nullptr) {
				SG::ErrorHelper::NumberExpected(Report, VisitorName, n.LeftValue->Location);
			}

			if(rnt == nullptr) {
				SG::ErrorHelper::NumberExpected(Report, VisitorName, n.RightValue->Location);
			}

			GenerateErrorResult();
		}
		else {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::NumberType());
		}
	}
	break;

	case InfixOperator::Greater:
	case InfixOperator::GreaterEqual:
	case InfixOperator::Less:
	case InfixOperator::LessEqual: {
		SG::NumberType* lnt = dynamic_cast<SG::NumberType*>(left_v.GeneratedExpressionType.get());
		SG::NumberType* rnt = dynamic_cast<SG::NumberType*>(right_v.GeneratedExpressionType.get());
		if(lnt == nullptr || rnt == nullptr) {
			if(lnt == nullptr) {
				SG::ErrorHelper::NumberExpected(Report, VisitorName, n.LeftValue->Location);
			}

			if(rnt == nullptr) {
				SG::ErrorHelper::NumberExpected(Report, VisitorName, n.RightValue->Location);
			}

			GenerateErrorResult();
		}
		else {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
		}
	}
	break;

	case InfixOperator::Equal:
	case InfixOperator::NotEqual: {
		// Can only compare numbers, enums, and boolean.
		SG::NumberType* lnt = dynamic_cast<SG::NumberType*>(left_v.GeneratedExpressionType.get());
		SG::BooleanType* lbt = dynamic_cast<SG::BooleanType*>(left_v.GeneratedExpressionType.get());
		SG::EnumerationType* let = dynamic_cast<SG::EnumerationType*>(left_v.GeneratedExpressionType.get());

		if(left_v.GeneratedExpressionType->CanAcceptValueOfType(*right_v.GeneratedExpressionType)
				&& right_v.GeneratedExpressionType->CanAcceptValueOfType(*left_v.GeneratedExpressionType)) {
			if(!lnt && !lbt && !let) {
				Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::TypesNotComparable, Diagnostics::ErrorLevel::Error,
						VisitorName, "operand type not comparable", n.Location));
				GenerateErrorResult();
			}
			else {
				GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
				GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
			}
		}
		else {
			Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ComparisonTypeMismatch, Diagnostics::ErrorLevel::Error,
					VisitorName, "cannot compare values of incompatible type", n.Location));
			GenerateErrorResult();
		}
	}
	break;

	case InfixOperator::And:
	case InfixOperator::Or: {
		SG::BooleanType* lnt = dynamic_cast<SG::BooleanType*>(left_v.GeneratedExpressionType.get());
		SG::BooleanType* rnt = dynamic_cast<SG::BooleanType*>(right_v.GeneratedExpressionType.get());
		if(lnt == nullptr || rnt == nullptr) {
			if(lnt == nullptr) {
				SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.LeftValue->Location);
			}

			if(rnt == nullptr) {
				SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.RightValue->Location);
			}

			GenerateErrorResult();
		}
		else {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
		}
	}
	break;

	case InfixOperator::Union:
	case InfixOperator::Intersect:
	case InfixOperator::SetMinus: {
		SG::SetType* lnt = dynamic_cast<SG::SetType*>(left_v.GeneratedExpressionType.get());
		SG::SetType* rnt = dynamic_cast<SG::SetType*>(right_v.GeneratedExpressionType.get());
		if(lnt == nullptr || rnt == nullptr) {
			if(lnt == nullptr) {
				SG::ErrorHelper::SetExpected(Report, VisitorName, n.LeftValue->Location);
			}

			if(rnt == nullptr) {
				SG::ErrorHelper::SetExpected(Report, VisitorName, n.RightValue->Location);
			}

			GenerateErrorResult();
		}
		else if(lnt->ElementType != rnt->ElementType) {
			SG::ErrorHelper::SetDomainMismatch(Report, VisitorName, n.Location);
			GenerateErrorResult();
		}
		else {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::SetType(lnt->ElementType));
		}
	}
	break;

	default:
		GenerateErrorResult();
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
				VisitorName, "unimplemented infix operator", n.Location));
		break;
	}
}

void ExpressionVisitor::VisitFunctionIfElseExpression(AST::FunctionIfElseExpression& n) {
	ExpressionVisitor pred_v(scope, Report);
	n.Predicate->Accept(pred_v);

	std::unique_ptr<SG::Expression> pred;
	SG::BooleanType t;
	if(t.CanAcceptValueOfType(*pred_v.GeneratedExpressionType)) {
		pred = std::move(pred_v.GeneratedExpression);
	}
	else {
		SG::ErrorHelper::BooleanExpected(Report, VisitorName, n.Location);
		pred = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
	}

	ExpressionVisitor if_v(scope, Report);
	n.Code->Accept(if_v);

	ExpressionVisitor else_v(scope, Report);
	n.ElseCode->Accept(else_v);

	if(if_v.GeneratedExpressionType->CanAcceptValueOfType(*else_v.GeneratedExpressionType)) {
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::FunctionIfElseExpression(pred, if_v.GeneratedExpression, else_v.GeneratedExpression));
		GeneratedExpressionType = std::move(if_v.GeneratedExpressionType);
	}
	else if(else_v.GeneratedExpressionType->CanAcceptValueOfType(*if_v.GeneratedExpressionType)) {
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::FunctionIfElseExpression(pred, if_v.GeneratedExpression, else_v.GeneratedExpression));
		GeneratedExpressionType = std::move(else_v.GeneratedExpressionType);
	}
	else {
		GenerateErrorResult();
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ConditionalDomainMismatch, Diagnostics::ErrorLevel::Error,
				VisitorName, "if/else domain mismatch", n.Location));
	}
}
