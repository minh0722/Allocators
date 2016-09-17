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

/*  
    void insertAfter(T* node)
    {
        remove();

        setPrev(node);
        setNext(node->getNext());

        node->setNext

        //setNext(node->getNext());
        //setPrev(node);

        //node->getNext()->setPrev(this);
        //node->setNext(this);
    }

    void insertBefore(T* node)
    {
        remove();

        setPrev(node->getPrev());
        setNext(node);

        node->getPrev()->setNext(this);
        node->setPrev(this);
    }
*/

    T* getData() const
    {
        return static_cast<T*>(this);
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
    2. If type is big, the blocks are of size > 4KB
    
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
    uint16_t convertToBlocksCount(size_t bytes);
    uint32_t convertToBytes(uint16_t blocks);

private:
    uintptr_t m_memory;
    uint32_t m_memorySize;
    uint16_t m_sizeOfEachBlocks;
    uint32_t m_freeBlocksCount;

    /// offset of current freed block from the start of the page in blocks(caused by free)
    int32_t m_currentFreedBlockOffset;

    /// offset of the start of the contiguous unallocated area from the start of the page in blocks
    uint16_t m_currentUnallocatedOffset;

};
