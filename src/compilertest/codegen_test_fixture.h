#pragma once

#include "runtime/code/functiontable.h"
#include "runtime/vm/virtualmachine.h"
#include "language_test_fixture.h"
#include <sstream>

class CodegenTestFixture : public LanguageTestFixture {
private:
	Deg::Runtime::Code::CodeBuffer code;

protected:
	std::stringstream code_text;

	CodegenTestFixture(const boost::filesystem::path& BasePath);

public:
	Deg::Runtime::Code::FunctionTable functionTable;
	Deg::Runtime::VM::VirtualMachine vm;

	virtual void ParseFiles(const std::vector<boost::filesystem::path>& files);
};
