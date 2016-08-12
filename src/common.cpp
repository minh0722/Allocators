#include "common.h"

size_t alignTo(const size_t size, const size_t alignment)
{
	return ((size + alignment - 1) / alignment) * alignment;
}