#pragma once
class FrameAllocator
{
public:
    FrameAllocator(size_t size);
    ~FrameAllocator();

    void* allocate(size_t size, size_t alignment);

    void reset();
private:
    void* m_start;
    void* m_end;
    void* m_current;
};

