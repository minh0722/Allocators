#pragma once
#include <cstdint>

template <typename T>
class LinkedListNode
{
public:
    LinkedListNode() : 
        m_next(this), m_prev(this) {}
    
    void remove()
    {
        m_next->setPrev(m_prev);
        m_prev->setNext(m_next);
    }

    void insertAfter(T* node)
    {
        remove();

        setNext(node->getNext());
        setPrev(node);

        node->getNext()->setPrev(this);
        node->setNext(this);
    }

    void insertBefore(T* node)
    {
        remove();

        setPrev(node->getPrev());
        setNext(node);

        node->getPrev()->setNext(this);
        node->setPrev(this);
    }

    T* getData() const
    {
        return static_cast<T*>(this);
    }

    LinkedListNode<T>* getNext()
    {
        return m_next;
    }

    LinkedListNode<T>* getPrev()
    {
        return m_prev;
    }

    void setNext(LinkedListNode<T>* next)
    {
        m_next = next;
    }

    void setPrev(LinkedListNode<T>* prev)
    {
        m_prev = prev;
    }

private:
    LinkedListNode<T>* m_next;
    LinkedListNode<T>* m_prev;
};

__declspec(align(128))
class PageBlock : public LinkedListNode<PageBlock>
{
public:
    PageBlock(uintptr_t memory, uint32_t sizeOfEachBlock, size_t memorySize = 4096);
    ~PageBlock();

    void* allocate();
    void free(void* ptr);

    uint16_t getSizeClass() const;

    bool hasFreeBlock() const;
    
    PageBlock() = delete;
    PageBlock(const PageBlock&) = delete;
    PageBlock& operator=(const PageBlock&) = delete;

private:
    uint16_t convertToBlocksCount(size_t bytes);
    uint32_t convertToBytes(uint16_t blocks);

private:
    uintptr_t m_memory;
    size_t m_memorySize;
    uint16_t m_sizeOfEachBlocks;
    uint32_t m_freedBlocksCount;

    /// offset of current freed block from the start of the page in blocks(caused by free)
    uint16_t m_currentFreedBlockOffset;

    /// offset of the start of the contiguous unallocated area from the start of the page in blocks
    uint16_t m_currentUnallocatedOffset;

};

