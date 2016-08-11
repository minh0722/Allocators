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
    /* @brief find the list that has blocks of size blockSize
    * @return list of blocks with size blockSize, nullptr if not found or list has no free block
    */
    Header* findList(size_t blockSize);

private:
    void* m_mem;
    void* m_end;
    std::vector<std::pair<size_t, IntrusiveLinkedListNode<Header>>> m_segList;
};
