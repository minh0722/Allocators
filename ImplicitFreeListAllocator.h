#pragma once

/*
    This allocator uses first-fit to find a free block
    and it uses only 1 header in each block, so free is linear.
    All allocation request are aligned on even alignment and
    all block sizes are even (due to all blocks are aligned to even addresses).
    The header is 64 bit size and the LSB is used for free/allocated
    flag. The block size is the header with the LSB cleared to 0.
*/

class ImplicitFreeListAllocator
{
public:
    ImplicitFreeListAllocator(size_t size);
    ~ImplicitFreeListAllocator();

    void* allocate(size_t size, size_t alignment);
    void free(void* ptr);

private:
    bool isAllocatedBlock(void* ptr);

    void setBlockAllocated(void* ptr, bool isAllocated);

    size_t blockSize(void* ptr);

    void setBlockSize(void* ptr, size_t size);

private:
    void* m_start;
    void* m_end;
    void* m_mem;       // pointer to the first valid header
};

