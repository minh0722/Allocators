#include "ImplicitListAllocator.h"

ImplicitListAllocator::ImplicitListAllocator(size_t size)
{
	m_size = alignTo(size, ALIGNMENT);

	m_start = VirtualAlloc(NULL, m_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	BlockHeader* header = (BlockHeader*)m_start;
	header->size = m_size;
	header->isAllocated = false;
}

ImplicitListAllocator::~ImplicitListAllocator()
{
	VirtualFree(m_start, 0, MEM_RELEASE);
}

void* ImplicitListAllocator::allocate(size_t size, size_t alignment)
{
	return nullptr;
}
