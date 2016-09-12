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
        ///void* blockMem;
    };

public:
    SegregatedListAllocator(size_t size);
    ~SegregatedListAllocator();

    void* allocate(size_t size);
    void free(void* ptr);

private:
    /** 
    * @brief find the list that has blocks of size blockSize
    * @return pair of index of the found list and the found list. If not found, return pair of -1 and nullptr
    */
    std::pair<size_t, Header*> findList(size_t blockSize);

    /** 
    * @brief find a free block header in the list
    * @return the free block header, nullptr if not found
    */
    Header* findFreeBlock(Header* freeList);

    /** 
    * @brief search for free block in other lists of bigger size than is requested.
    * @param startIndex the list index from which to start searching
    * @param blockSize the block size that we are requesting
    * @return the free block, nullptr if not found
    */
    Header* searchForFreeBlockInLists(size_t startIndex, size_t blockSize);

    /**
    * @brief insert the block into the appropriate list of its size
    * @param blockHeader the block to insert
    */
    void insertBlockInAppropriateList(Header* blockHeader);

private:
    void* m_mem;
    void* m_end;

    // std::list instead of vector?
    std::vector<std::pair<size_t, Header*>> m_segList;
};
