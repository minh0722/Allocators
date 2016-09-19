#pragma once
#include <cstdint>

template <typename T>
class LinkedListNode
{
public:
    LinkedListNode() : 
        m_next(nullptr), m_prev(nullptr) {}
    
    void remove()
    {
        if (m_next)
        {
            m_next->setPrev(m_prev);
        }
        if(m_prev)
        {
            m_prev->setNext(m_next);
        }
    }

    T* getNext()
    {
        return m_next;
    }

    T* getPrev()
    {
        return m_prev;
    }

    void setNext(T* next)
    {
        m_next = next;
    }

    void setPrev(T* prev)
    {
        m_prev = prev;
    }

private:
    T* m_next;
    T* m_prev;
};

/*
    Page blocks are 512KB blocks of memory, divided to smaller blocks, whose size depends on the type:
    1. If type is small, the blocks are 4KB
    2. If type is big, the blocks are of size > 2KB
    
*/

__declspec(align(128))
class PageBlock : public LinkedListNode<PageBlock>
{
public:
    PageBlock(uintptr_t memory, uint32_t memorySize, uint32_t sizeOfEachBlock);
    ~PageBlock();

    void* allocate();
    void free(void* ptr);

    uint16_t getSizeClass() const;
    uint32_t getFreeBlocksCount() const;

    bool hasFreeBlock() const;
    
    PageBlock() = delete;
    PageBlock(const PageBlock&) = delete;
    PageBlock& operator=(const PageBlock&) = delete;

private:
    uint32_t convertToBlocksCount(uint32_t bytes);
    uint32_t convertToBytes(int32_t blocks);

private:
    uintptr_t m_memory;
    uint32_t m_memorySize;
    uint32_t m_sizeOfEachBlocks;
    uint32_t m_freeBlocksCount;

    /// offset of current freed block from the start of the page in blocks(caused by free)
    int32_t m_currentFreedBlockOffset;

    /// offset of the start of the contiguous unallocated area from the start of the page in blocks
    int32_t m_currentUnallocatedOffset;

};

