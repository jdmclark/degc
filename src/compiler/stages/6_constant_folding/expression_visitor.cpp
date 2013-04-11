#include "expression_visitor.h"

using namespace Deg::Compiler::SG;
using Deg::Compiler::Stages::ConstantFolding::ExpressionVisitor;

ExpressionVisitor::ExpressionVisitor(std::vector<std::unique_ptr<SG::Expression>>& FunctionArguments, Diagnostics::Report& report)
	: SG::Visitor("ConstantFolding::ExpressionVisitor", report), FunctionArguments(FunctionArguments), ResultFoldable(false) {
	return;
}

bool ExpressionVisitor::GetConstantBoolean() {
	SG::BooleanExpression& exp = dynamic_cast<SG::BooleanExpression&>(*GeneratedExpression);
	return exp.Value;
}

Deg::Runtime::Math::DefaultFixed ExpressionVisitor::GetConstantNumber() {
	SG::NumericExpression& exp = dynamic_cast<SG::NumericExpression&>(*GeneratedExpression);
	return exp.Value;
}

Deg::Compiler::SG::RecordSymbol* ExpressionVisitor::GetConstantSetSymbol() {
	SG::TypedSetExpression* tse = dynamic_cast<SG::TypedSetExpression*>(GeneratedExpression.get());
	if(tse != nullptr) {
		return tse->ElementType;
	}

	SG::ConstrainedSetExpression& cse = dynamic_cast<SG::ConstrainedSetExpression&>(*GeneratedExpression);
	return cse.ElementType;
}

std::unique_ptr<Deg::Compiler::SG::Expression> ExpressionVisitor::GetConstantSetFilter() {
	SG::TypedSetExpression* tse = dynamic_cast<SG::TypedSetExpression*>(GeneratedExpression.get());
	if(tse != nullptr) {
		return std::unique_ptr<SG::Expression>(new BooleanExpression(true));
	}

	SG::ConstrainedSetExpression& cse = dynamic_cast<SG::ConstrainedSetExpression&>(*GeneratedExpression);
	ExpressionVisitor v(FunctionArguments, Report);
	cse.Filter->Accept(v);
	return std::move(v.GeneratedExpression);
}

Deg::Compiler::Diagnostics::ErrorLocation ExpressionVisitor::GetConstantSetLocation() {
	SG::TypedSetExpression* tse = dynamic_cast<SG::TypedSetExpression*>(GeneratedExpression.get());
	if(tse != nullptr) {
		return tse->Location;
	}

	SG::ConstrainedSetExpression& cse = dynamic_cast<SG::ConstrainedSetExpression&>(*GeneratedExpression);
	ExpressionVisitor v(FunctionArguments, Report);
	cse.Filter->Accept(v);
	return cse.Location;
}

bool ExpressionVisitor::GetConstantEquals(SG::Expression& a, SG::Expression& b) {
	SG::NumericExpression* na = dynamic_cast<SG::NumericExpression*>(&a);
	SG::NumericExpression* nb = dynamic_cast<SG::NumericExpression*>(&b);
	if(na && nb) {
		return na->Value == nb->Value;
	}

	SG::BooleanExpression* ba = dynamic_cast<SG::BooleanExpression*>(&a);
	SG::BooleanExpression* bb = dynamic_cast<SG::BooleanExpression*>(&b);
	if(ba && bb) {
		return ba->Value == bb->Value;
	}

	SG::IdentifierExpression* ia = dynamic_cast<SG::IdentifierExpression*>(&a);
	SG::IdentifierExpression* ib = dynamic_cast<SG::IdentifierExpression*>(&b);
	if(ia && ib) {
		SG::EnumerationMemberSymbol* ea = dynamic_cast<SG::EnumerationMemberSymbol*>(ia->ReferencedNode);
		SG::EnumerationMemberSymbol* eb = dynamic_cast<SG::EnumerationMemberSymbol*>(ib->ReferencedNode);

		if(ea && eb) {
			return ea->Value == eb->Value;
		}
	}

	throw std::exception();
}

void ExpressionVisitor::DefaultAction(const std::string& action, SG::Node& n) {
	SG::Visitor::DefaultAction(action, n);
	throw std::exception();
}

void ExpressionVisitor::VisitProgramSymbol(SG::ProgramSymbol& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	ResultFoldable = true;
}

void ExpressionVisitor::VisitProgramArgumentSymbol(SG::ProgramArgumentSymbol& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	ResultFoldable = false;
}

void ExpressionVisitor::VisitFunctionArgumentSymbol(SG::FunctionArgumentSymbol& n) {
	ExpressionVisitor v(FunctionArguments, Report);
	FunctionArguments[n.Index]->Accept(v);
	GeneratedExpression = std::move(v.GeneratedExpression);
	ResultFoldable = v.ResultFoldable;
}

void ExpressionVisitor::VisitFunctionSymbol(SG::FunctionSymbol& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	ResultFoldable = true;
}

void ExpressionVisitor::VisitEnumerationMemberSymbol(SG::EnumerationMemberSymbol& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	ResultFoldable = true;
}

void ExpressionVisitor::VisitRecordMemberSymbol(SG::RecordMemberSymbol& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::IdentifierExpression(&n));
	ResultFoldable = false;
}

void ExpressionVisitor::VisitNumericExpression(SG::NumericExpression& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(n.Value));
	ResultFoldable = true;
}

void ExpressionVisitor::VisitBooleanExpression(SG::BooleanExpression& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(n.Value));
	ResultFoldable = true;
}

void ExpressionVisitor::VisitTypedSetExpression(SG::TypedSetExpression& n) {
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::TypedSetExpression(n.ElementType, n.Location));
	ResultFoldable = true;
}

void ExpressionVisitor::VisitConstrainedSetExpression(SG::ConstrainedSetExpression& n) {
	ExpressionVisitor filter_v(FunctionArguments, Report);
	n.Filter->Accept(filter_v);
	GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ConstrainedSetExpression(n.ElementType, filter_v.GeneratedExpression, n.Location));
	ResultFoldable = true;
}

void ExpressionVisitor::VisitPanicExpression(SG::PanicExpression& n) {
	// Not a constant expression
	throw std::exception();
}

void ExpressionVisitor::VisitIdentifierExpression(SG::IdentifierExpression& n) {
	n.ReferencedNode->Accept(*this);
}

void ExpressionVisitor::VisitFunctionCallExpression(SG::FunctionCallExpression& n) {
	ExpressionVisitor fn_v(FunctionArguments, Report);
	n.FunctionTargetExpression->Accept(fn_v);

	SG::IdentifierExpression& fn_exp = dynamic_cast<SG::IdentifierExpression&>(*fn_v.GeneratedExpression);
	SG::FunctionSymbol& fn_sym = dynamic_cast<SG::FunctionSymbol&>(*fn_exp.ReferencedNode);

	std::vector<std::unique_ptr<SG::Expression>> args;

	for(auto& arg : n.ArgumentExpressions) {
		ExpressionVisitor arg_v(FunctionArguments, Report);
		arg->Accept(arg_v);
		args.push_back(std::move(arg_v.GeneratedExpression));
	}

	ExpressionVisitor value_v(args, Report);
	fn_sym.Code->Accept(value_v);
	GeneratedExpression = std::move(value_v.GeneratedExpression);
	ResultFoldable = value_v.ResultFoldable;
}

void ExpressionVisitor::VisitUnaryExpression(SG::UnaryExpression& n) {
	ExpressionVisitor value_v(FunctionArguments, Report);
	n.Value->Accept(value_v);

	if(!value_v.ResultFoldable) {
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::UnaryExpression(value_v.GeneratedExpression, n.Operator));
		ResultFoldable = false;
		return;
	}

	switch(n.Operator) {
	case AST::UnaryOperator::Minus:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(-value_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::UnaryOperator::Not:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(!value_v.GetConstantBoolean()));
		ResultFoldable = true;
		break;

	default:
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
				VisitorName, "unimplemented unary operator"));
		throw std::exception();
	}
}

void ExpressionVisitor::VisitExistsExpression(SG::ExistsExpression& n) {
	// Not a constant expression
	throw std::exception();
}

void ExpressionVisitor::VisitInfixExpression(SG::InfixExpression& n) {
	ExpressionVisitor left_v(FunctionArguments, Report);
	n.LeftValue->Accept(left_v);
	ExpressionVisitor right_v(FunctionArguments, Report);
	n.RightValue->Accept(right_v);

	if(!left_v.ResultFoldable || !right_v.ResultFoldable) {
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::InfixExpression(left_v.GeneratedExpression, right_v.GeneratedExpression, n.Operator));
		ResultFoldable = false;
		return;
	}

	switch(n.Operator) {
	case AST::InfixOperator::Addition:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(left_v.GetConstantNumber() + right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Subtraction:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(left_v.GetConstantNumber() - right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Multiplication:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(left_v.GetConstantNumber() * right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Division:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::NumericExpression(left_v.GetConstantNumber() / right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Greater:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(left_v.GetConstantNumber() > right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::GreaterEqual:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(left_v.GetConstantNumber() >= right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Less:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(left_v.GetConstantNumber() < right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::LessEqual:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(left_v.GetConstantNumber() <= right_v.GetConstantNumber()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Equal:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(GetConstantEquals(*left_v.GeneratedExpression, *right_v.GeneratedExpression)));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::NotEqual:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(!GetConstantEquals(*left_v.GeneratedExpression, *right_v.GeneratedExpression)));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::And:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(left_v.GetConstantBoolean() && right_v.GetConstantBoolean()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Or:
		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::BooleanExpression(left_v.GetConstantBoolean() || right_v.GetConstantBoolean()));
		ResultFoldable = true;
		break;

	case AST::InfixOperator::Union: {
		auto p = left_v.GetConstantSetFilter();
		auto q = right_v.GetConstantSetFilter();
		std::unique_ptr<SG::Expression> filter_exp(new SG::InfixExpression(p, q, AST::InfixOperator::Or));
		ExpressionVisitor filter_exp_v(FunctionArguments, Report);
		filter_exp->Accept(filter_exp_v);

		Diagnostics::ErrorLocation left_loc = left_v.GetConstantSetLocation();
		Diagnostics::ErrorLocation right_loc = right_v.GetConstantSetLocation();

		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ConstrainedSetExpression(left_v.GetConstantSetSymbol(), filter_exp_v.GeneratedExpression,
				Diagnostics::ErrorLocation(left_loc.filename, left_loc.first_line, left_loc.first_column, right_loc.last_line, right_loc.last_column)));
		ResultFoldable = true;
	}
	break;

	case AST::InfixOperator::Intersect: {
		auto p = left_v.GetConstantSetFilter();
		auto q = right_v.GetConstantSetFilter();
		std::unique_ptr<SG::Expression> filter_exp(new SG::InfixExpression(p, q, AST::InfixOperator::And));
		ExpressionVisitor filter_exp_v(FunctionArguments, Report);
		filter_exp->Accept(filter_exp_v);

		Diagnostics::ErrorLocation left_loc = left_v.GetConstantSetLocation();
		Diagnostics::ErrorLocation right_loc = right_v.GetConstantSetLocation();

		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ConstrainedSetExpression(left_v.GetConstantSetSymbol(), filter_exp_v.GeneratedExpression,
				Diagnostics::ErrorLocation(left_loc.filename, left_loc.first_line, left_loc.first_column, right_loc.last_line, right_loc.last_column)));
		ResultFoldable = true;
	}
	break;

	case AST::InfixOperator::SetMinus: {
		auto p = left_v.GetConstantSetFilter();
		auto q = right_v.GetConstantSetFilter();
		std::unique_ptr<SG::Expression> filter_neg(new SG::UnaryExpression(q, AST::UnaryOperator::Not));
		std::unique_ptr<SG::Expression> filter_exp(new SG::InfixExpression(p, q, AST::InfixOperator::And));
		ExpressionVisitor filter_exp_v(FunctionArguments, Report);
		filter_exp->Accept(filter_exp_v);

		Diagnostics::ErrorLocation left_loc = left_v.GetConstantSetLocation();
		Diagnostics::ErrorLocation right_loc = right_v.GetConstantSetLocation();

		GeneratedExpression = std::unique_ptr<SG::Expression>(new SG::ConstrainedSetExpression(left_v.GetConstantSetSymbol(), filter_exp_v.GeneratedExpression,
				Diagnostics::ErrorLocation(left_loc.filename, left_loc.first_line, left_loc.first_column, right_loc.last_line, right_loc.last_column)));
		ResultFoldable = true;
	}
	break;

	default:
		Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
				VisitorName, "unimplemented infix operator"));
		throw std::exception();
	}
}

void ExpressionVisitor::VisitFunctionIfElseExpression(SG::FunctionIfElseExpression& n) {
	ExpressionVisitor v(FunctionArguments, Report);
	n.Predicate->Accept(v);

	bool branch = v.GetConstantBoolean();
	if(branch) {
		ExpressionVisitor bv(FunctionArguments, Report);
		n.IfCode->Accept(bv);
		GeneratedExpression = std::move(bv.GeneratedExpression);
		ResultFoldable = bv.ResultFoldable;
	}
	else {
		ExpressionVisitor bv(FunctionArguments, Report);
		n.ElseCode->Accept(bv);
		GeneratedExpression = std::move(bv.GeneratedExpression);
		ResultFoldable = bv.ResultFoldable;
	}
}
