#include "recordtypetable.h"

Deg::Runtime::Code::RecordTypeInfo::RecordTypeInfo(size_t type_id, size_t width, size_t quantity)
	: type_id(type_id), width(width), quantity(quantity) {
	return;
}
