#include "ImplicitFreeListAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>
#include <cassert>
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
    setBlockSize(m_mem, size - sizeof(size_t));
}


ImplicitFreeListAllocator::~ImplicitFreeListAllocator()
{
    free(m_start);
}

void* ImplicitFreeListAllocator::allocate(size_t size, size_t alignment)
{
    size = align(size, 2);
    const size_t HEADER_SIZE = sizeof(size_t);

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

            setBlockSize(alignedMem + size, currentBlockSize - size - HEADER_SIZE);

            return alignedMem;
        }

        while (alignedMemHeader < currentBlockMem + 2 && alignedMem != currentBlockMem)
        {
            alignedMem += alignment;
            alignedMemHeader = alignedMem - HEADER_SIZE;
        }
       


        // split the block
        setBlockSize(currentBlockHeader, alignedMemHeader - currentBlockMem);

        setBlockSize(alignedMemHeader, size);
        setBlockAllocated(alignedMemHeader, true);
                
        setBlockSize(alignedMem + size, currentBlockSize - blockSize(currentBlockHeader) - HEADER_SIZE - size);        

    }

    return nullptr;













    //// find the block that is big enough and it's free
    //while (currentBlockHeader < m_end && 
    //        (isAllocatedBlock(currentBlockHeader) || 
    //        blockSize(currentBlockHeader) < size || 
    //        align(size, 2) + HEADER_SIZE + 2 > blockSize(currentBlockHeader) || 
    //        align(size, 2) != blockSize(currentBlockHeader)))
    //{
    //    currentBlockHeader += (HEADER_SIZE + blockSize(currentBlockHeader));
    //}

    //if (currentBlockHeader >= m_end)
    //{
    //    return nullptr;
    //}
    //
    //// get the block memory pointer
    //uint8_t* blockMem = currentBlockHeader + HEADER_SIZE;
    //
    //// first case: this block alignment is good
    //uint8_t* alignedBlockMem = (uint8_t*)align((uintptr_t)currentBlockMem, alignment);
    //
    //if (alignedBlockMem == currentBlockMem)
    //{
    //    // split the block and return the pointer
    //}


    //// second case: search for next aligned block. Have to be careful with the header
    //uint8_t* alignedBlockHeader = (uint8_t*)align((uintptr_t)currentBlockMem, alignment) - HEADER_SIZE;
    //
    //while (alignedBlockHeader == alignedBlockMem || alignedBlockHeader < currentBlockHeader)
    //{
    //    alignedBlockHeader += alignment;
    //}
    //
    //if (alignedBlockHeader + HEADER_SIZE + size >= m_end)
    //{
    //    return nullptr;
    //}
    //
    //size_t leftBlockSize = alignedBlockHeader - currentBlockMem;
    //setBlockSize(currentBlockHeader, leftBlockSize);
    
}

void ImplicitFreeListAllocator::free(void * ptr)
{
}

bool ImplicitFreeListAllocator::isAllocatedBlock(void * ptr)
{
    // LSB = 0 - free
    // LSB = 1 - allocated

    return (uintptr_t)ptr & 1;
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
    *(size_t*)ptr |= size;
}
