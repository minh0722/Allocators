#pragma once

class ImplicitFreeListAllocator
{
public:
    ImplicitFreeListAllocator(size_t size);
    ~ImplicitFreeListAllocator();

    void* allocate(size_t size, size_t alignment);
    void free(void* ptr);

private:
    bool isAllocatedBlock(void* ptr);

    size_t blockSize(void* ptr);

    void setBlockSize(void* ptr, size_t size);

private:
    void* m_start;
    void* m_end;
    void* m_mem;       // pointer to the first valid header
};

