#pragma once

#include <nullunit/nullunit.h>
#include <boost/filesystem/path.hpp>

#include "compiler/diagnostics/storedreport.h"
#include "compiler/grammar/instance.h"

class SyntacticTestFixture : public NullUnit::Fixture {
public:
	Deg::Compiler::Diagnostics::StoredReport Report;
	const boost::filesystem::path BasePath;

protected:
	SyntacticTestFixture(const boost::filesystem::path& BasePath);

public:
	virtual void ParseFile(const boost::filesystem::path& filename);
	void PrintErrors() const;
};

#define AssertResult(n_errors, n_warnings) {												\
	Test_Expect_Eq(Report.GetErrorCount(), n_errors);										\
	Test_Expect_Eq(Report.GetWarningCount(), n_warnings);									\
	if(Report.GetErrorCount() != n_errors || Report.GetWarningCount() != n_warnings) {		\
		PrintErrors();																		\
	}																						\
}
