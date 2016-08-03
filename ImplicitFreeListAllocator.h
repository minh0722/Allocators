#pragma once

class ImplicitFreeListAllocator
{
public:
    ImplicitFreeListAllocator(size_t size);
    ~ImplicitFreeListAllocator();

    void* allocate(size_t size, size_t alignment);
    void free(void* ptr);

private:
    bool isFreeBlock(void* ptr);

private:
    void* m_start;
    void* m_end;
};

