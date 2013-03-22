#pragma once

#include <vector>
#include <stddef.h>

namespace Deg {
namespace Runtime {
namespace Code {

class CodeBuffer {
private:
	std::vector<unsigned char> data;
	size_t topPtr;

	void throwCodeBufferOverflowException() const;

	inline void resizeDataForWrite(size_t dest, size_t size) {
		if((dest + size) > topPtr) {
			topPtr = dest + size;
		}

		if(topPtr >= data.size()) {
			data.resize((topPtr + size) * 2, 0);
		}
	}

	inline void tryRead(size_t src, size_t size) const {
		if((src + size) > topPtr) {
			throwCodeBufferOverflowException();
		}
	}

public:
	CodeBuffer();

	void Write(size_t dest, const void* src, size_t size);
	void Read(void* dest, size_t src, size_t size) const;

	template <typename T> inline void Write(T value, size_t dest) {
		resizeDataForWrite(dest, sizeof(T));
		*(T*)(&data[dest]) = value;
	}

	template <typename T> inline T Read(size_t src) const {
		tryRead(src, sizeof(T));
		return *(T*)(&data[src]);
	}

	inline void* WritePointer(size_t dest, size_t size) {
		resizeDataForWrite(dest, size);
		return (void*)&data[dest];
	}

	inline const void* ReadPointer(size_t src, size_t size) const {
		tryRead(src, size);
		return (const void*)&data[src];
	}

	inline size_t Size() const {
		return topPtr;
	}
};

}
}
}
