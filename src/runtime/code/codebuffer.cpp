#include "codebuffer.h"
#include <cstring>

Deg::Runtime::Code::CodeBuffer::CodeBuffer()
	: data(128, 0), topPtr(0)
{
	return;
}

void Deg::Runtime::Code::CodeBuffer::throwCodeBufferOverflowException() const
{
	throw std::exception();
}

void Deg::Runtime::Code::CodeBuffer::Write(size_t dest, const void* src, size_t size)
{
	resizeDataForWrite(dest, size);
	memcpy(&data[dest], src, size);
}

void Deg::Runtime::Code::CodeBuffer::Read(void* dest, size_t src, size_t size) const
{
	tryRead(src, size);
	memcpy(dest, &data[src], size);
}
