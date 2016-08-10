#pragma once

class SegregatedListAllocator
{
public:
    SegregatedListAllocator(size_t size);
    ~SegregatedListAllocator();

private:
    void* m_mem;
    void* m_end;
};

