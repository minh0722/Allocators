#include "ImplicitFreeListAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>
#include "util.h"

ImplicitFreeListAllocator::ImplicitFreeListAllocator(size_t size)
{
    void* mem = malloc(size);

    if (!mem)
        throw std::exception("Can't allocate memory");

    m_start = mem;
    m_end = (uint8_t*)mem + size;

    // setting the header for the initial block
    *(size_t*)m_start = size - sizeof(size_t);
}


ImplicitFreeListAllocator::~ImplicitFreeListAllocator()
{
    free(m_start);
}

void* ImplicitFreeListAllocator::allocate(size_t size, size_t alignment)
{

}

void ImplicitFreeListAllocator::free(void * ptr)
{
}

bool ImplicitFreeListAllocator::isFreeBlock(void * ptr)
{
    // LSB = 0 - free
    // LSB = 1 - allocated

    return !((uintptr_t)ptr & 0x1);
}
