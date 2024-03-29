#pragma once

#include "language_test_fixture.h"

class SyntacticTestFixture : public LanguageTestFixture {
protected:
	SyntacticTestFixture(const boost::filesystem::path& BasePath);

public:
	virtual void ParseFiles(const std::vector<boost::filesystem::path>& files);
};
