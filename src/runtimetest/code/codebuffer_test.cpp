#include <nullunit/nullunit.h>
#include "runtime/code/codebuffer.h"

#include <string>
#include <cstring>

using namespace Deg::Runtime::Code;

BeginSuite(CodeBufferTests);

Case(Write) {
	CodeBuffer cb;

	std::string hello = "Hello, World!\n";

	cb.Write(0, hello.c_str(), static_cast<size_t>(hello.length()));
	cb.Write(96, hello.c_str(), static_cast<size_t>(hello.length()));
}

Case(WriteTemplated) {
	CodeBuffer cb;
	cb.Write<int>(0xFEEDBEEF, 5);
}

Case(Read) {
	CodeBuffer cb;

	int value = 0x0EEDBEEF;

	cb.Write(0, &value, sizeof(int));

	int readValue = 0;

	cb.Read(&readValue, 0, sizeof(int));

	Test_Assert_Eq(readValue, 0x0EEDBEEF);
}

Case(ReadFailure) {
	CodeBuffer cb;

	int value = 0xFEEDBEEF;

	cb.Write(0, &value, sizeof(int));

	try {
		int readValue = 0;
		cb.Read(&readValue, 2, sizeof(int));
	}
	catch(std::exception&) {
		return;
	}

	Test_Assert_Always("Code buffer overflow exception not thrown.");
}

Case(ReadTemplated) {
	CodeBuffer cb;
	cb.Write<int>(0x0EEDBEEF, 5);
	Test_Assert_Eq(cb.Read<int>(5), 0x0EEDBEEF);
}

Case(ReadTemplatedFailure) {
	CodeBuffer cb;
	cb.Write<int>(0xFEEDBEEF, 4);
	try {
		cb.Read<int>(5);
	}
	catch(std::exception&) {
		return;
	}

	Test_Assert_Always("Code buffer overflow exception not thrown.");
}

Case(ReadPointer) {
	CodeBuffer cb;
	cb.Write<int>(0x0EEDBEEF, 0);
	const void* rp = cb.ReadPointer(0, sizeof(int));

	int readValue = 0;
	memcpy(&readValue, rp, sizeof(int));

	Test_Assert_Eq(readValue, 0x0EEDBEEF);
}

Case(WritePointer) {
	CodeBuffer cb;
	void* wp = cb.WritePointer(0, sizeof(int));

	int writeValue = 0x0EEDBEEF;
	memcpy(wp, &writeValue, sizeof(int));

	Test_Assert_Eq(cb.Read<int>(0), 0x0EEDBEEF);
}

Case(Size) {
	CodeBuffer cb;
	cb.Write<int>(0xFEEDBEEF, 0);
	cb.Write<int>(0xDEADBEEF, sizeof(int));
	cb.Write<int>(0xFEEDDEAD, sizeof(int) * 2);

	Test_Assert_Eq(cb.Size(), sizeof(int) * 3);
}

EndSuite(CodeBufferTests);
