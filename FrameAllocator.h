#pragma once
class FrameAllocator
{
public:
    FrameAllocator(size_t size);
    ~FrameAllocator();

private:
    void* m_start;
    void* m_end;
};

