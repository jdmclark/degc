#include <nullunit/nullunit.h>
#include <sstream>

#include "compiler/diagnostics/storedreport.h"

using namespace Deg::Compiler::Diagnostics;

class StoredReportTestFixture : public NullUnit::Fixture {
public:
	StoredReport report;
};

BeginSuiteFixture(StoredReportTest, StoredReportTestFixture);

Case(AddError) {
	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 0);

	report.AddError(Error(ErrorCode::SyntaxError, ErrorLevel::Error, "Stage 0", "Reason", ErrorLocation("test.deg", 15, 0, 16, 11)));
	ErrorLocation l("test.deg", 15, 0, 16, 11);

	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 1);

	auto it = report.begin();
	Test_Assert_Eq(it->Code, ErrorCode::SyntaxError);
	Test_Assert_Eq(it->Level, ErrorLevel::Error);
	Test_Assert_Eq(it->Stage, "Stage 0");
	Test_Assert_Eq(it->Location, l);
	Test_Assert_Eq(it->Reason, "Reason");
}

Case(AddWarning) {
	Test_Assert_Eq(report.GetErrorCount(), 0);
	Test_Assert_Eq(report.GetWarningCount(), 0);

	report.AddError(Error(ErrorCode::SyntaxError, ErrorLevel::Warning, "Stage 0", "Reason", ErrorLocation("test.deg", 15, 0, 16, 11)));
	ErrorLocation l("test.deg", 15, 0, 16, 11);

	Test_Assert_Eq(report.GetErrorCount(), 0);
	Test_Assert_Eq(report.GetWarningCount(), 1);

	auto it = report.begin();
	Test_Assert_Eq(it->Code, ErrorCode::SyntaxError);
	Test_Assert_Eq(it->Level, ErrorLevel::Warning);
	Test_Assert_Eq(it->Stage, "Stage 0");
	Test_Assert_Eq(it->Location, l);
	Test_Assert_Eq(it->Reason, "Reason");
}

Case(AddCriticalError) {
	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 0);

	report.AddError(Error(ErrorCode::FeatureNotImplemented, ErrorLevel::CriticalError, "Stage 0", "Reason"));

	Test_Assert_Eq(report.GetWarningCount(), 0);
	Test_Assert_Eq(report.GetErrorCount(), 1);

	auto it = report.begin();
	Test_Assert_Eq(it->Code, ErrorCode::FeatureNotImplemented);
	Test_Assert_Eq(it->Level, ErrorLevel::CriticalError);
	Test_Assert_Eq(it->Stage, "Stage 0");
	Test_Assert_Eq(it->Location.filename, nullptr);
	Test_Assert_Eq(it->Reason, "Reason");
}

EndSuite(StoredReportTest);
