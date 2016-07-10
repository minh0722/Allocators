#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(size_t size)
{
	size = alignTo(size, ALIGNMENT);
	m_start = reinterpret_cast<uint8_t*>(VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
	m_size = size;
	m_currentSize = 0;
}

LinearAllocator::~LinearAllocator()
{
	VirtualFree(m_start, 0, MEM_RELEASE);
}

void* LinearAllocator::allocate(_In_ size_t sizeInByte, _In_opt_ size_t alignment)
{
	if (m_currentSize >= m_size)
		return nullptr;
	
	sizeInByte = alignTo(sizeInByte, alignment);

	if (m_currentSize + sizeInByte > m_size)
		return nullptr;

	void* ptr = m_start + m_currentSize;
	m_currentSize += sizeInByte;
	return ptr;
}
