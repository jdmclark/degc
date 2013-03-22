#pragma once

#include "codebuffer.h"

namespace Deg {
namespace Runtime {
namespace Code {

class CodeBufferReadStream {
private:
	const CodeBuffer& codeBuffer;
	size_t streamPtr;

public:
	explicit CodeBufferReadStream(const CodeBuffer& cb);

	inline void Read(void* dest, size_t size) {
		codeBuffer.Read(dest, streamPtr, size);
		streamPtr += size;
	}

	template <typename T> inline T Read() {
		T value = codeBuffer.Read<T>(streamPtr);
		streamPtr += sizeof(T);
		return value;
	}

	inline void Seek(size_t dest) {
		streamPtr = dest;
	}

	inline void Scan(int offset) {
		streamPtr += offset;
	}

	inline size_t Tell() const {
		return streamPtr;
	}
};

}
}
}
