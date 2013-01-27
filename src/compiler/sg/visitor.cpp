#include "visitor.h"
#include <boost/format.hpp>

Deg::Compiler::SG::Visitor::Visitor(const std::string& name, Diagnostics::Report& report)
	: VisitorName(name), Report(report) {
	return;
}

Deg::Compiler::SG::Visitor::~Visitor() {
	return;
}

void Deg::Compiler::SG::Visitor::DefaultAction(const std::string& action, Node& n) {
	Report.AddError(Diagnostics::Error(Diagnostics::ErrorCode::FeatureNotImplemented, Diagnostics::ErrorLevel::CriticalError,
			VisitorName, boost::str(boost::format("feature not implemented: %s") % action)));
}

SGCLASS_ALL(SGDEFAULTACTION)
