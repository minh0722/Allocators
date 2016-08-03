#include "ImplicitFreeListAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>
#include "util.h"

ImplicitFreeListAllocator::ImplicitFreeListAllocator(size_t size)
{
    size = align(size, 2);

    void* mem = malloc(size);

    if (!mem)
        throw std::exception("Can't allocate memory");

    m_start = mem;
    m_end = (uint8_t*)mem + size;
    m_mem = m_start;

    // setting the header for the initial block
    // the LSB = 0 by default
    *(size_t*)m_mem = size - sizeof(size_t);
}


ImplicitFreeListAllocator::~ImplicitFreeListAllocator()
{
    free(m_start);
}

void* ImplicitFreeListAllocator::allocate(size_t size, size_t alignment)
{
    uint8_t* blockHeader = (uint8_t*)m_mem;

    while (blockHeader < m_end && (isAllocatedBlock(blockHeader) || blockSize(blockHeader) < size))
    {
        blockHeader += (sizeof(size_t) + blockSize(blockHeader));
    }

    if (blockHeader >= m_end)
    {
        return nullptr;
    }
    
    // get the block memory
    uint8_t* blockMem = blockHeader + sizeof(size_t);
    uint8_t* alignedBlockMem = (uint8_t*)align((uintptr_t)blockMem, alignment);

    if (alignedBlockMem + size >= m_end)
    {
        return nullptr;
    }

    if (blockMem != alignedBlockMem)
    {

    }

}

void ImplicitFreeListAllocator::free(void * ptr)
{
}

bool ImplicitFreeListAllocator::isAllocatedBlock(void * ptr)
{
    // LSB = 0 - free
    // LSB = 1 - allocated

    return (uintptr_t)ptr & 0x1;
}

size_t ImplicitFreeListAllocator::blockSize(void * ptr)
{
    return *(size_t*)ptr & -2;
}

void ImplicitFreeListAllocator::setBlockSize(void* ptr, size_t size)
{
    *(size_t*)ptr &= (size + 1);
}
