#include "SegregatedListAllocator.h"
#include "util.h"
#include <cstdlib>
#include <exception>
#include <algorithm>

SegregatedListAllocator::SegregatedListAllocator(size_t size)
{
    align(size, 2);

    uint8_t* mem = (uint8_t*)malloc(size);

    if (!mem)
    {
        throw std::exception("Can't allocate memory");
    }

    m_mem = mem;
    m_end = mem + size;

    size_t blockSize = size - sizeof(Header);
    //uint8_t* blockMem = (uint8_t*)m_mem + sizeof(Header);

    Header* header = (Header*)m_mem;
    new(header) Header(blockSize/*, blockMem*/);

    m_segList.emplace_back(make_pair(blockSize, IntrusiveLinkedListNode<Header>()));

    header->node.insertAfter(&m_segList[0].second);
}


SegregatedListAllocator::~SegregatedListAllocator()
{
    ::free(m_mem);
}

void SegregatedListAllocator::allocate(size_t size)
{
    size = align(size, 2);

    Header* freeList = findList(size);

    if (freeList)
    {
        // find free block
        Header* freeBlock = freeList;
        freeBlock->node.remove();

        
    }
    else
    {
        // if can't find free block, look in the next lists
        // if even after that no free list is found then split from big chunk and create new list from it
    }
}

SegregatedListAllocator::Header* SegregatedListAllocator::findList(size_t blockSize)
{
    size_t left = 0;
    size_t right = m_segList.size() - 1;
    size_t mid = (left + right) / 2;

    while (m_segList[mid].first != blockSize && left < right)
    {
        if (blockSize > m_segList[mid].first)
        {
            left = mid + 1;
            mid = (left + right) / 2;
        }
        else
        {
            right = mid;
            mid = (left + right) / 2;
        }
    }

    if (m_segList[mid].first == blockSize)
        return m_segList[mid].second;

    return nullptr;
}

SegregatedListAllocator::Header::Header()
{
    blockSize = 0;
    //blockMem = nullptr;
    node.setOwner(this);
}

SegregatedListAllocator::Header::Header(size_t size/*, void * mem*/)
{
    blockSize = size;
    //blockMem = mem;
    node.setOwner(this);
}
