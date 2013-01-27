#pragma once

#include <nullunit/nullunit.h>
#include <boost/filesystem/path.hpp>
#include <vector>

#include "compiler/diagnostics/storedreport.h"
#include "compiler/grammar/instance.h"

class LanguageTestFixture : public NullUnit::Fixture {
public:
	Deg::Compiler::Diagnostics::StoredReport Report;
	const boost::filesystem::path BasePath;

protected:
	LanguageTestFixture(const boost::filesystem::path& BasePath);

public:
	virtual void ParseFiles(const std::vector<boost::filesystem::path>& files) = 0;

	inline void ParseFile(const boost::filesystem::path& filename) {
		ParseFiles({filename});
	}

	void PrintErrors() const;
};

#define AssertResult(n_errors, n_warnings) {												\
	Test_Expect_Eq(Report.GetErrorCount(), n_errors);										\
	Test_Expect_Eq(Report.GetWarningCount(), n_warnings);									\
	if(Report.GetErrorCount() != n_errors || Report.GetWarningCount() != n_warnings) {		\
		PrintErrors();																		\
	}																						\
}
