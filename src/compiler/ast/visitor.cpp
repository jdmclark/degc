#include "visitor.h"
#include <boost/format.hpp>

Deg::Compiler::AST::Visitor::Visitor(const std::string& name, Diagnostics::Report& report)
	: VisitorName(name), Report(report) {
	return;
}

Deg::Compiler::AST::Visitor::~Visitor() {
	return;
}

void Deg::Compiler::AST::Visitor::DefaultAction(const std::string& action, Node& n) {
	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
			VisitorName, boost::str(boost::format("feature not implemented: %s") % action)));
}

ASTCLASS_ALL(AVDEFAULTACTION)
