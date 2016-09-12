#include "SegregatedListAllocator.h"
#include "util.h"
#include <cstdlib>
#include <exception>
#include <algorithm>
#include <cassert>

SegregatedListAllocator::SegregatedListAllocator(size_t size)
{
    align(size, 2);

    uint8_t* mem = (uint8_t*)::operator new(size, std::nothrow);

    if (!mem)
    {
        throw std::exception("Can't allocate memory");
    }

    m_mem = mem;
    m_end = mem + size;

    size_t blockSize = size - sizeof(Header);
    ///uint8_t* blockMem = (uint8_t*)m_mem + sizeof(Header);

    Header* header = (Header*)m_mem;
    new(header) Header(blockSize/*, blockMem*/);

    /// create first list with one root
    m_segList.emplace_back(make_pair(blockSize, new Header()));
    
    /// insert header into the list
    header->node.insertAfter(&m_segList[0].second->node);
}


SegregatedListAllocator::~SegregatedListAllocator()
{
    ::free(m_mem);

    for (auto& freeList : m_segList)
        delete freeList.second;
}

void* SegregatedListAllocator::allocate(size_t size)
{
    size = align(size, 2);

    std::pair<size_t, Header*> freeListLocation = findList(size);
    size_t freeListIndex = freeListLocation.first;
    Header* freeList = freeListLocation.second;
    
    if (freeList)
    {
        /// find free block
        Header* freeBlock = findFreeBlock(freeList);

        /// free block was found
        if (freeBlock)
        {
            void* mem = freeBlock + 1;
            freeBlock->node.remove();

            return mem;
        }
    }
    
    /// if can't find free block, look in the next lists and split
    return searchForFreeBlockInLists(freeListIndex + 1, size);    
}

void SegregatedListAllocator::free(void* ptr)
{
    
}

std::pair<size_t, SegregatedListAllocator::Header*> SegregatedListAllocator::findList(size_t blockSize)
{
    for (size_t i = 0; i < m_segList.size(); ++i)
    {
        if (m_segList[i].first == blockSize)
            return make_pair(i, m_segList[i].second);
    }

    return make_pair(-1, nullptr);
}

SegregatedListAllocator::Header* SegregatedListAllocator::findFreeBlock(Header* freeList)
{
    if (freeList->node.isEmpty())
    {
        return nullptr;
    }

    return freeList->node.nextOwner();
}

SegregatedListAllocator::Header* SegregatedListAllocator::searchForFreeBlockInLists(size_t startIndex, size_t blockSize)
{
    assert(!(blockSize & 1));

    size_t minimumBlockSize = blockSize + sizeof(Header) + 2;

    for (auto& segList : m_segList)
    {
        size_t listBlockSize = segList.first;
        Header* listHead = segList.second;

        if (listBlockSize >= minimumBlockSize)
        {
            Header* freeBlock = findFreeBlock(listHead);

            if (!freeBlock)
            {
                continue;
            }

            // remove the block from current list
            freeBlock->node.remove();

            // split the block
            Header* splittedBlock = (Header*)((uint8_t*)(freeBlock + 1) + blockSize);
            size_t splittedBlockSize = listBlockSize - blockSize - sizeof(Header);
            new(splittedBlock) Header(splittedBlockSize);

            // set the free block size
            new(freeBlock) Header(blockSize);

            // insert the splitted block into the appropriate list
            insertBlockInAppropriateList(splittedBlock);

            return freeBlock + 1;
        }
    }

    return nullptr;
}

void SegregatedListAllocator::insertBlockInAppropriateList(Header* blockHeader)
{
    size_t blockSize = blockHeader->blockSize;

    // find the list with blocks of the same size
    std::pair<size_t, Header*> listLocation = findList(blockSize);

    Header* appropriateList = listLocation.second;

    if (appropriateList)
    {
        appropriateList->node.addToEnd(&blockHeader->node);
    }
    else
    {
        /// make new list
        m_segList.emplace_back(make_pair(blockSize, new Header()));
        //m_segList.back().second->node.addToEnd(&blockHeader->node);
        blockHeader->node.addToEnd(&m_segList.back().second->node);
    }
}

SegregatedListAllocator::Header::Header()
{
    blockSize = 0;
    node.setOwner(this);
    ///blockMem = nullptr;
}

SegregatedListAllocator::Header::Header(size_t size/*, void * mem*/)
{
    blockSize = size;
    node.setOwner(this);
    ///blockMem = mem;
}