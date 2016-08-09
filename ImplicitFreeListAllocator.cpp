#include "ImplicitFreeListAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>
#include <cassert>
#include <cstring>
#include "util.h"

const size_t HEADER_SIZE = sizeof(size_t);
const size_t SMALLEST_BLOCK_SIZE = 2;

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
    assert(!(alignment & 1));
    size = align(size, SMALLEST_BLOCK_SIZE);

    uint8_t* currentBlockHeader = (uint8_t*)m_mem;
    uint8_t* currentBlockMem = currentBlockHeader + HEADER_SIZE;
    uint8_t* nextBlockHeader = currentBlockMem + blockSize(currentBlockHeader);

    while (currentBlockHeader < m_end)
    {
        size_t currentBlockSize = blockSize(currentBlockHeader);
        uint8_t* alignedMemHeader = (uint8_t*)alignHeader((uintptr_t)currentBlockHeader, HEADER_SIZE, alignment);
        uint8_t* alignedMem = alignedMemHeader + HEADER_SIZE;

        size_t allocatedBlockAndHeaderSize = HEADER_SIZE + size;

        //if (alignedMem == currentBlockMem && size == currentBlockSize)
        //{
        //    setBlockAllocated(currentBlockHeader, true);
        //    return currentBlockMem;
        //}

        // make sure that we can split to the left
        while (alignedMemHeader < currentBlockMem + SMALLEST_BLOCK_SIZE && alignedMemHeader != currentBlockHeader)
        {
            alignedMemHeader += (HEADER_SIZE + alignment);
            alignedMem = alignedMemHeader + HEADER_SIZE;
        }

        // make sure that we can split to the right
        if (isAllocatedBlock(currentBlockHeader) ||
            currentBlockSize < size ||
            alignedMemHeader + allocatedBlockAndHeaderSize + (HEADER_SIZE + SMALLEST_BLOCK_SIZE) >= nextBlockHeader)     // if we can't split to the right
        {
            currentBlockHeader = nextBlockHeader;
            currentBlockMem = currentBlockHeader + HEADER_SIZE;
            nextBlockHeader = currentBlockMem + blockSize(currentBlockHeader);
            continue;
        }

        if (alignedMemHeader == currentBlockHeader)
        {
            setBlockSize(alignedMemHeader, size);
            setBlockAllocated(alignedMemHeader, true);

            // split to the right
            setBlockSize(alignedMemHeader + allocatedBlockAndHeaderSize, currentBlockSize - size - HEADER_SIZE);

            return alignedMemHeader + HEADER_SIZE;
        }

        // set the left block
        setBlockSize(currentBlockHeader, alignedMemHeader - currentBlockHeader - HEADER_SIZE);

        // set this block
        setBlockSize(alignedMemHeader, size);
        setBlockAllocated(alignedMemHeader, true);

        // set the right block
        setBlockSize(alignedMemHeader + allocatedBlockAndHeaderSize, currentBlockSize - blockSize(currentBlockHeader) - allocatedBlockAndHeaderSize - HEADER_SIZE);

        return alignedMem;
    }

    return nullptr;    
}

void ImplicitFreeListAllocator::free(void * ptr)
{
    cout << "free" << endl;
    if (!ptr)
    {
        return;
    }

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

template <typename T>
void ImplicitFreeListAllocator::free(void* ptr)
{
    cout << "template free" << endl;
    if (!ptr)
    {
        return;
    }

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

        // call destructor on the object
        reinterpret_cast<T*>(currentBlockMem)->~T();

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


template void ImplicitFreeListAllocator::free<A>(void* ptr);
template void ImplicitFreeListAllocator::free<B>(void* ptr);