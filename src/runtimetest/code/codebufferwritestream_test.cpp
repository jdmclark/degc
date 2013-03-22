#include <nullunit/nullunit.h>
#include "runtime/code/codebufferwritestream.h"
#include "runtime/code/codebufferreadstream.h"

using namespace Deg::Runtime::Code;

BeginSuite(CodeBufferWriteStreamTests);

Case(Seek)
{
	CodeBuffer cBuffer;
	CodeBufferWriteStream cb(cBuffer);

	cb.Seek(128);
	Test_Assert_Eq(cb.Tell(), 128);
}

Case(WriteStream)
{
	CodeBuffer cBuffer;
	CodeBufferWriteStream cb(cBuffer);

	std::string hello = "Hello, World!\n";

	cb.Write(hello.c_str(), static_cast<unsigned int>(hello.length()));
	cb.Write(hello.c_str(), static_cast<unsigned int>(hello.length()));
}

Case(WriteStreamTemplated)
{
	CodeBuffer cBuffer;
	CodeBufferWriteStream cb(cBuffer);

	cb.Write<int>(0xFEEDBEEF);
	cb.Write<int>(0xABACADAB);
	cb.Write<int>(0xF0F0F0F0);
}

Case(WriteBuffer)
{
	CodeBuffer cBufferOne;
	cBufferOne.Write<int>(0x0EEDBEEF, 0);

	CodeBuffer cBuffer;
	CodeBufferWriteStream cb(cBuffer);
	size_t destOffset = cb.WriteBuffer(cBufferOne);
	size_t destOffset2 = cb.WriteBuffer(cBufferOne);

	Test_Assert_Eq(cBuffer.Read<int>(0), 0x0EEDBEEF);
	Test_Assert_Eq(cBuffer.Read<int>(sizeof(int)), 0x0EEDBEEF);
	Test_Assert_Eq(destOffset, 0);
	Test_Assert_Eq(destOffset2, sizeof(int));
}

EndSuite(CodeBufferWriteStreamTests);
