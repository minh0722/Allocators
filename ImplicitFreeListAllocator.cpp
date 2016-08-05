#include "ImplicitFreeListAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>
#include <cassert>
#include <cstring>
#include "util.h"

const size_t HEADER_SIZE = sizeof(size_t);

ImplicitFreeListAllocator::ImplicitFreeListAllocator(size_t size)
{
    size = align(size, 2);

    void* mem = malloc(size);

    if (!mem)
        throw std::exception("Can't allocate memory");

    memset(mem, 0, size);

    m_start = mem;
    m_end = (uint8_t*)mem + size;
    m_mem = (void*)align((uintptr_t)mem, 2);

    // setting the header for the initial block
    // the LSB = 0 by default
    setBlockSize(m_mem, size - HEADER_SIZE);
}


ImplicitFreeListAllocator::~ImplicitFreeListAllocator()
{
    ::free(m_start);
}

void* ImplicitFreeListAllocator::allocate(size_t size, size_t alignment)
{
    size = align(size, 2);

    uint8_t* currentBlockHeader = (uint8_t*)m_mem;
    uint8_t* currentBlockMem = currentBlockHeader + HEADER_SIZE;
    uint8_t* nextBlockHeader = currentBlockMem + blockSize(currentBlockHeader);

    while (currentBlockHeader < m_end)
    {
        size_t currentBlockSize = blockSize(currentBlockHeader);
        uint8_t* alignedMem = (uint8_t*)align((uintptr_t)currentBlockMem, alignment);
        uint8_t* alignedMemHeader = alignedMem - HEADER_SIZE;

        //if (alignedMem == currentBlockMem && size == currentBlockSize)
        //{
        //    setBlockAllocated(currentBlockHeader, true);
        //    return currentBlockMem;
        //}

        // make sure that we can split to the left
        while (alignedMemHeader < currentBlockMem + 2 && alignedMem != currentBlockMem)
        {
            alignedMem += alignment;
            alignedMemHeader = alignedMem - HEADER_SIZE;
        }

        if (isAllocatedBlock(currentBlockHeader) ||
            currentBlockSize < size ||
            alignedMem + size + HEADER_SIZE + 2 >= nextBlockHeader)     // if we can't split to the right
        {
            currentBlockHeader = nextBlockHeader;
            currentBlockMem = currentBlockHeader + HEADER_SIZE;
            nextBlockHeader = currentBlockMem + blockSize(currentBlockHeader);
            continue;
        }

        if (alignedMem == currentBlockMem)
        {
            setBlockSize(alignedMemHeader, size);
            setBlockAllocated(alignedMemHeader, true);

            // split to the right
            setBlockSize(alignedMem + size, currentBlockSize - size - HEADER_SIZE);

            return alignedMem;
        }

        // split the block
        setBlockSize(currentBlockHeader, alignedMemHeader - currentBlockMem);

        setBlockSize(alignedMemHeader, size);
        setBlockAllocated(alignedMemHeader, true);
                
        setBlockSize(alignedMem + size, currentBlockSize - blockSize(currentBlockHeader) - HEADER_SIZE - size - HEADER_SIZE);        

        return alignedMem;
    }

    return nullptr;    
}

void ImplicitFreeListAllocator::free(void * ptr)
{
    uint8_t* prevBlockHeader = nullptr;
    uint8_t* prevBlockMem = nullptr;
    uint8_t* currentBlockHeader = (uint8_t*)m_mem;
    uint8_t* currentBlockMem = currentBlockHeader + HEADER_SIZE;
    uint8_t* nextBlockHeader = currentBlockMem + blockSize(currentBlockHeader);
    uint8_t* nextBlockMem = nextBlockHeader + HEADER_SIZE;

    while (currentBlockHeader != m_end)
    {
        if (currentBlockMem != ptr)
        {
            prevBlockHeader = currentBlockHeader;
            prevBlockMem = currentBlockMem;
            currentBlockHeader = nextBlockHeader;
            currentBlockMem = nextBlockMem;
            nextBlockHeader = currentBlockMem + blockSize(currentBlockHeader);
            nextBlockMem = nextBlockHeader + HEADER_SIZE;
            continue;
        }

        // coalesce with the right block
        if (!isAllocatedBlock(nextBlockHeader))
        {
            size_t totalSize = blockSize(currentBlockHeader) + HEADER_SIZE + blockSize(nextBlockHeader);
            setBlockSize(currentBlockHeader, totalSize);
            setBlockAllocated(currentBlockHeader, false);
        }

        // coalesce with the left block
        if (prevBlockHeader != nullptr && prevBlockMem != nullptr && !isAllocatedBlock(prevBlockHeader))
        {
            size_t totalSize = blockSize(prevBlockHeader) + HEADER_SIZE + blockSize(currentBlockHeader);

            setBlockSize(prevBlockHeader, totalSize);
            setBlockAllocated(prevBlockHeader, false);
        }
        break;
    }
}

bool ImplicitFreeListAllocator::isAllocatedBlock(void * ptr)
{
    // LSB = 0 - free
    // LSB = 1 - allocated

    return *(size_t*)ptr & 1;
}

void ImplicitFreeListAllocator::setBlockAllocated(void* ptr, bool isAllocated)
{
    *(size_t*)ptr |= 1;
}

size_t ImplicitFreeListAllocator::blockSize(void * ptr)
{
    return *(size_t*)ptr & -2;
}

void ImplicitFreeListAllocator::setBlockSize(void* ptr, size_t size)
{
    assert(!(size & 1));
    
    // clear size bits
    *(size_t*)ptr &= 1;
    *(size_t*)ptr |= size;
}
