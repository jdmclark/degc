#include <nullunit/nullunit.h>
#include <sstream>

#include "compiler/diagnostics/streamreport.h"

using namespace Deg::Compiler::Diagnostics;

class StreamReportTestFixture : public NullUnit::Fixture {
public:
	std::ostringstream dummystream;
	StreamReport report;

	StreamReportTestFixture() : report(dummystream) {
		return;
	}
};

BeginSuiteFixture(StreamReportTest, StreamReportTestFixture);

Case(AddError) {
	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 0);

	report.AddError(Error(ErrorCode::SyntaxError, ErrorLevel::Error, "Stage 0", "Reason", ErrorLocation("test.deg", 15, 0, 16, 11)));

	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 1);
}

Case(AddWarning) {
	Test_Assert_Eq(report.GetErrorCount(), 0);
	Test_Assert_Eq(report.GetWarningCount(), 0);

	report.AddError(Error(ErrorCode::SyntaxError, ErrorLevel::Warning, "Stage 0", "Reason", ErrorLocation("test.deg", 15, 0, 16, 11)));

	Test_Assert_Eq(report.GetErrorCount(), 0);
	Test_Assert_Eq(report.GetWarningCount(), 1);
}

Case(AddCriticalError) {
	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 0);

	report.AddError(Error(ErrorCode::FeatureNotImplemented, ErrorLevel::CriticalError, "Stage 0", "Reason"));

	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 1);
}

EndSuite(StreamReportTest);
