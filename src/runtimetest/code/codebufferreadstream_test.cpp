#include <nullunit/nullunit.h>
#include "runtime/code/codebufferreadstream.h"
#include "runtime/code/codebufferwritestream.h"

using namespace Deg::Runtime::Code;

BeginSuite(CodeBufferReadStreamTests);

Case(Seek)
{
	CodeBuffer cBuffer;
	CodeBufferReadStream cb(cBuffer);

	cb.Seek(128);
	Test_Assert_Eq(cb.Tell(), 128);
}

Case(ReadStream)
{
	CodeBuffer cBuffer;
	CodeBufferWriteStream cb(cBuffer);
	CodeBufferReadStream db(cBuffer);

	int value = 0x0EEDBEEF;

	cb.Write(&value, sizeof(int));

	int readValue = 0;

	db.Read(&readValue, sizeof(int));

	Test_Assert_Eq(readValue, 0x0EEDBEEF);
}

Case(ReadStreamTemplated)
{
	CodeBuffer cBuffer;
	CodeBufferWriteStream cb(cBuffer);
	CodeBufferReadStream db(cBuffer);

	cb.Write<int>(0x0EEDBEEF);
	cb.Write<int>(0x0BACADAB);
	cb.Write<int>(0x00F0F0F0);

	Test_Assert_Eq(db.Read<int>(), 0x0EEDBEEF);
	Test_Assert_Eq(db.Read<int>(), 0x0BACADAB);
	Test_Assert_Eq(db.Read<int>(), 0x00F0F0F0);
}

EndSuite(CodeBufferReadStreamTests);
