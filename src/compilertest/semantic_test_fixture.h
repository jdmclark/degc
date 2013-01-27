#pragma once

#include "language_test_fixture.h"

class SemanticTestFixture : public LanguageTestFixture {
protected:
	SemanticTestFixture(const boost::filesystem::path& BasePath);

public:
	virtual void ParseFiles(const std::vector<boost::filesystem::path>& files);
};
