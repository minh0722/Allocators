#pragma once
#include "IntrusiveLinkedList.h"
#include <vector>
#include <utility>

class SegregatedListAllocator
{
    struct Header
    {
        Header();
        Header(size_t size/*, void* mem*/);

        IntrusiveLinkedListNode<Header> node;
        size_t blockSize;
        //void* blockMem;
    };

public:
    SegregatedListAllocator(size_t size);
    ~SegregatedListAllocator();

    void allocate(size_t size);

private:
    void* m_mem;
    void* m_end;
    std::vector<std::pair<size_t,Header*>> m_segList;
};
