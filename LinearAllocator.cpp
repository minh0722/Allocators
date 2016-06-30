#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(size_t size)
{
	m_start = reinterpret_cast<uint8_t*>(VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
}

LinearAllocator::~LinearAllocator()
{
	VirtualFree(m_start, 0, MEM_RELEASE);
}
