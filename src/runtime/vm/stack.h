#pragma once

#include <vector>
#include <stddef.h>

namespace Deg {
namespace Runtime {
namespace VM {

class Stack {
private:
	std::vector<unsigned char> data;
	size_t topPtr;

	void throwStackUnderrunException() const;

	inline void resizeDataForWrite(size_t size) {
		if(data.size() <= topPtr + size) {
			data.resize((topPtr + size) * 2, 0);
		}
	}

public:
	Stack();

	template <typename T> inline void Push(T value) {
		resizeDataForWrite(sizeof(T));
		*(T*)(&data[topPtr]) = value;
		topPtr += sizeof(T);
	}

	template <typename T> inline T Pop() {
		if(topPtr < sizeof(T)) {
			throwStackUnderrunException();
		}

		topPtr -= sizeof(T);
		return *(T*)(&data[topPtr]);
	}
};

}
}
}
