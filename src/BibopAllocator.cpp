#include "BibopAllocator.h"
#include "PageBlock.h"
#include "util.h"

#define GRANULARITY 4
#define DIFF_OF_SIZE_CLASS 4
#define PAGE_BLOCK_SIZE 4096
#define MAX_ALLOCATION_SIZE (PAGE_BLOCK_SIZE / 2)

BibopAllocator::BibopAllocator()
{
}


BibopAllocator::~BibopAllocator()
{
}

void* BibopAllocator::allocate(uint32_t size)
{
    if (!size)
        return nullptr;
        
    if (size < GRANULARITY)
        size = GRANULARITY;
    

    if (size > MAX_ALLOCATION_SIZE)
    {
        size = static_cast<uint32_t>(align(size + sizeof(PageBlock), PAGE_BLOCK_SIZE));

        uint8_t* mem = new uint8_t[size];
        uintptr_t pageBlockStart = reinterpret_cast<uintptr_t>(mem + sizeof(PageBlock));

        new(mem) PageBlock(pageBlockStart, size, size);

        m_bibopTable.registerBigPageBlocks(pageBlockStart, 1);

        return reinterpret_cast<void*>(pageBlockStart);
    }
    
    /// each size class differs from each other by 4 bytes
    size = static_cast<uint32_t>(align(size, DIFF_OF_SIZE_CLASS));
    
    void* result = nullptr;

    for (auto pageBlock : m_pageBlocks)
    {
        if (pageBlock->getSizeClass() == size)
        {
            result = pageBlock->allocate();

            if (!result)
            {
                /// create another page of this size class
                PageBlock* newPageBlock = createNewPageBlock(size);

                /// put the new page block to front of the list


                result = newPageBlock->allocate();
            }

            break;
        }
        else
        {
            /// create new page block
            PageBlock* newPageBlock = createNewPageBlock(size);
            
            result = newPageBlock->allocate();
        }
    }

    return result;
}

void BibopAllocator::free(void * pointer)
{
    BibopTag tag = m_bibopTable.getBibopTag(pointer);
    PageBlock* pageBlock = m_bibopTable.getPageBlock(pointer);

    if (tag.isLargePageBlock())
    {
        delete pageBlock;
    }
    else
    {
        pageBlock->free(pointer);
    }

}

PageBlock* BibopAllocator::createNewPageBlock(uint32_t sizeClass)
{
    uint8_t* mem = new uint8_t[PAGE_BLOCK_SIZE];
    uintptr_t pageBlockStart = reinterpret_cast<uintptr_t>(mem + sizeof(PageBlock));

    new(mem) PageBlock(pageBlockStart, sizeClass, PAGE_BLOCK_SIZE);
    m_bibopTable.registerSmallPageBlocks(reinterpret_cast<uintptr_t>(mem), pageBlockStart, PAGE_BLOCK_SIZE / sizeClass);

    return reinterpret_cast<PageBlock*>(mem);
}
