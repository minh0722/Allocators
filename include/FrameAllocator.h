#pragma once
struct Deleter
{
    void (*cleanup)(void* ptr);
    Deleter* nextDeleter;
};

class FrameAllocator
{
public:
    FrameAllocator(size_t size);
    ~FrameAllocator();

    void* allocate(size_t size, size_t alignment);

    template <typename T>
    T* allocateObject(size_t alignment);
    
    void reset();
private:
    void* m_start;
    void* m_end;
    void* m_current;

    Deleter* m_deleters;
};