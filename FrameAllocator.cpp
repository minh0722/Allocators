#include "FrameAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>
#include "util.h"

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
    uintptr_t alignedMem = align((uintptr_t)m_current, alignment);

    if (alignedMem + size > (uintptr_t)m_end)
        return nullptr;

    m_current = (uint8_t*)alignedMem + size;

    return (void*)alignedMem;
}

template<typename T>
T* FrameAllocator::allocateObject(size_t size, size_t alignment)
{
    
}

void FrameAllocator::reset()
{
    m_current = m_start;
}
