#include "BibopAllocator.h"
#include "PageBlock.h"
#include "util.h"
#include <Windows.h>

#define GRANULARITY 4
#define DIFF_OF_SIZE_CLASS 4
#define PAGE_SIZE 4096
#define PAGE_BLOCK_SIZE 512 * 1024
#define SMALL_ALLOCATION_SIZE (2048)

BibopAllocator::BibopAllocator()
{
    m_bibopTable = new BibopTable(0x0);
}

BibopAllocator::~BibopAllocator()
{
    delete m_bibopTable;

    for (auto pageBlock : m_pageBlocks)
    {
        destroyPageBlock(pageBlock);
    }
}

void* BibopAllocator::allocate(uint32_t size)
{
    if (!size)
        return nullptr;
        
    if (size < GRANULARITY)
        size = GRANULARITY;
    

    if (size > SMALL_ALLOCATION_SIZE)
    {
        void* mem = VirtualAlloc(nullptr, size + sizeof(PageBlock), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        void* firstBlock = reinterpret_cast<uint8_t*>(mem) + sizeof(PageBlock);

        new(mem) PageBlock(reinterpret_cast<uintptr_t>(firstBlock), size, size);
        m_bibopTable->registerBigPageBlocks(firstBlock, 1);

        return reinterpret_cast<uint8_t*>(mem) + sizeof(PageBlock);

        //size = static_cast<uint32_t>(align(size + sizeof(PageBlock), PAGE_SIZE));
        //
        //PageBlock* newPageBlock = createNewPageBlock(size, size);
        //m_pageBlocks.push_back(newPageBlock);
        //
        //uintptr_t firstBlock = reinterpret_cast<uintptr_t>(newPageBlock + 1);
        //m_bibopTable->registerBigPageBlocks(firstBlock, 1);
        //
        //return reinterpret_cast<void*>(newPageBlock + 1);
    }
    
    /// each size class differs from each other by 4 bytes
    size = static_cast<uint32_t>(align(size, DIFF_OF_SIZE_CLASS));
    
    void* result = nullptr;

    for (uint32_t i = 0; i < m_pageBlocks.size(); ++i)
    {
        if (m_pageBlocks[i]->getSizeClass() == size)
        {
            result = m_pageBlocks[i]->allocate();

            if (!result)
            {
                /// create another page of this size class
                PageBlock* newPageBlock = createNewPageBlock(size, PAGE_BLOCK_SIZE);

                uintptr_t firstBlock = reinterpret_cast<uintptr_t>(newPageBlock + 1);

                m_bibopTable->registerSmallPageBlocks(newPageBlock, firstBlock, PAGE_BLOCK_SIZE / PAGE_SIZE);

                /// put the new page block to front of the list
                newPageBlock->setNext(m_pageBlocks[i]);
                m_pageBlocks[i]->setPrev(newPageBlock);
                m_pageBlocks[i] = newPageBlock;

                return newPageBlock->allocate();
            }

            return result;
        }
    }

    /// create new page block
    PageBlock* newPageBlock = createNewPageBlock(size, PAGE_BLOCK_SIZE);

    uintptr_t firstBlock = reinterpret_cast<uintptr_t>(newPageBlock + 1);

    m_bibopTable->registerSmallPageBlocks(newPageBlock, firstBlock, PAGE_BLOCK_SIZE / PAGE_SIZE);

    m_pageBlocks.push_back(newPageBlock);
    
    return newPageBlock->allocate();
}

void BibopAllocator::free(void* pointer)
{
    BibopTag tag = m_bibopTable->getBibopTag(pointer);
    PageBlock* pageBlock = m_bibopTable->getPageBlock(pointer);

    if (tag.isLargePageBlock())
    {
        VirtualFree(pageBlock, 0, MEM_RELEASE);
    }
    else
    {
        pageBlock->free(pointer);
    }

}

PageBlock* BibopAllocator::createNewPageBlock(uint32_t sizeClass, uint32_t blockSize)
{
    uint8_t* mem = reinterpret_cast<uint8_t*>(VirtualAlloc(nullptr, PAGE_BLOCK_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
    uintptr_t pageBlockStart = reinterpret_cast<uintptr_t>(mem + sizeof(PageBlock));

    new(mem) PageBlock(pageBlockStart, blockSize - sizeof(PageBlock), sizeClass);

    return reinterpret_cast<PageBlock*>(mem);
}

void BibopAllocator::destroyPageBlock(PageBlock* pageBlock)
{
    PageBlock* currentPageBlock = pageBlock;

    while (currentPageBlock)
    {
        PageBlock* next = currentPageBlock->getNext();
        currentPageBlock->~PageBlock();
        VirtualFree(currentPageBlock, 0, MEM_RELEASE);

        currentPageBlock = next;
    }
}
