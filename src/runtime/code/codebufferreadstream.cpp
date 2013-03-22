#include "codebufferreadstream.h"

Deg::Runtime::Code::CodeBufferReadStream::CodeBufferReadStream(const CodeBuffer& cBuffer)
	: codeBuffer(cBuffer), streamPtr(0) {
	return;
}
