#include "codebufferwritestream.h"

Deg::Runtime::Code::CodeBufferWriteStream::CodeBufferWriteStream(Deg::Runtime::Code::CodeBuffer& cBuffer)
	: codeBuffer(cBuffer), streamPtr(0) {
	return;
}
