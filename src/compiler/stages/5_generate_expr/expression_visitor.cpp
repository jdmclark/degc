#include "expression_visitor.h"
#include "compiler/sg/error_helper.h"
#include "identifier_visitor.h"
#include <boost/format.hpp>

using namespace Deg::Compiler::AST;
using Deg::Compiler::Stages::GenerateExpressions::ExpressionVisitor;

ExpressionVisitor::ExpressionVisitor(SG::ScopeStack& scope, Diagnostics::Report& report)
	: AST::Visitor("GenerateExpressions::ExpressionVisitor", report), scope(scope), IsConstantValue(false) {
	return;
}

void ExpressionVisitor::GenerateErrorResult() {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ErrorExpression());
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::ErrorType());
}

void ExpressionVisitor::VisitNumericLiteralExpression(AST::NumericLiteralExpression& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(n.Value));
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::NumberType());
}

void ExpressionVisitor::VisitBooleanLiteralExpression(AST::BooleanLiteralExpression& n) {
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(n.Value));
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
}

void ExpressionVisitor::VisitTypedSetExpression(AST::TypedSetExpression& n) {
	IsConstantValue = true;

	try {
		SG::Node& element_node = scope.GetSymbol(n.Typename);
		SG::RecordSymbol* rec_node = dynamic_cast<SG::RecordSymbol*>(&element_node);
		if(rec_node) {
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::TypedSetExpression(rec_node));
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
	IsConstantValue = true;

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

			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ConstrainedSetExpression(rec_node, filter));
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
	IsConstantValue = true;
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::PanicExpression());
	GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::ErrorType());
}

void ExpressionVisitor::VisitIdentifierExpression(AST::IdentifierExpression& n) {
	try {
		SG::Node& node = scope.GetSymbol(n.Identifier);
		IdentifierVisitor v(scope, n.Identifier, n.Location, Report);
		node.Accept(v);
		IsConstantValue = v.IsConstantValue;
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

		IsConstantValue = false;
		GeneratedExpression = std::move(fexp);
		GeneratedExpressionType = fn_type->ReturnType->Clone();

		return;
	}

	GenerateErrorResult();
	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FunctionCallToNonFunction, Diagnostics::ErrorLevel::Error,
			VisitorName, "attempted function call to non-function value", n.Location));
}

void ExpressionVisitor::VisitMemberAccessExpression(AST::MemberAccessExpression& n) {
	// TODO: This needs to be implemented.
	GenerateErrorResult();
	DefaultAction("MemberAccessExpression", n);
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
			// TODO: Add constant folding here.
			IsConstantValue = false;
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
			// TODO: Add constant folding here.
			IsConstantValue = false;
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
			// TODO: Add constant folding here.
			IsConstantValue = false;
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
			// TODO: Add constant folding here.
			IsConstantValue = false;
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
		}
	}
	break;

	case InfixOperator::Equal:
	case InfixOperator::NotEqual: {
		if(left_v.GeneratedExpressionType->CanAcceptValueOfType(*right_v.GeneratedExpressionType)
				&& right_v.GeneratedExpressionType->CanAcceptValueOfType(*left_v.GeneratedExpressionType)) {
			// TODO: Add constant folding here.
			IsConstantValue = false;
			GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
			GeneratedExpressionType = std::unique_ptr<SG::Type>(new SG::BooleanType());
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
			// TODO: Add constant folding here.
			IsConstantValue = false;
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
			// TODO: Add constant folding here.
			IsConstantValue = false;
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
	ExpressionVisitor if_v(scope, Report);
	n.Code->Accept(if_v);

	ExpressionVisitor else_v(scope, Report);
	n.ElseCode->Accept(else_v);

	if(if_v.GeneratedExpressionType->CanAcceptValueOfType(*else_v.GeneratedExpressionType)) {
		IsConstantValue = false;
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::FunctionIfElseExpression(if_v.GeneratedExpression, else_v.GeneratedExpression));
		GeneratedExpressionType = std::move(if_v.GeneratedExpressionType);
	}
	else if(else_v.GeneratedExpressionType->CanAcceptValueOfType(*if_v.GeneratedExpressionType)) {
		IsConstantValue = false;
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::FunctionIfElseExpression(if_v.GeneratedExpression, else_v.GeneratedExpression));
		GeneratedExpressionType = std::move(else_v.GeneratedExpressionType);
	}
	else {
		GenerateErrorResult();
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::ConditionalDomainMismatch, Diagnostics::ErrorLevel::Error,
				VisitorName, "if/else domain mismatch", n.Location));
	}
}
