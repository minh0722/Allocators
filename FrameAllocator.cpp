#include "FrameAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>

FrameAllocator::FrameAllocator(size_t size)
{
    void* mem = malloc(size);

    if (!mem)
        throw std::exception("Can't allocate memory");
    
    m_start = mem;
    m_end = (uint8_t*)mem + size;
    m_current = m_start;
}


FrameAllocator::~FrameAllocator()
{
    free(m_start);
}

void* FrameAllocator::allocate(size_t size, size_t alignment)
{
    return nullptr;
}

void FrameAllocator::reset()
{
    m_current = m_start;
}
