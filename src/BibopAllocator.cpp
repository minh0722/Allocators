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
    for (auto pageBlock : m_pageBlocks)
    {
        pageBlock->~PageBlock();
    }
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

        PageBlock* newPageBlock = createNewPageBlock(size, size);
        m_pageBlocks.push_back(newPageBlock);
        
        uintptr_t firstBlock = reinterpret_cast<uintptr_t>(newPageBlock + 1);
        m_bibopTable.registerBigPageBlocks(firstBlock, 1);

        return reinterpret_cast<void*>(newPageBlock + 1);
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
                PageBlock* newPageBlock = createNewPageBlock(size, PAGE_BLOCK_SIZE);

                uintptr_t firstBlock = reinterpret_cast<uintptr_t>(newPageBlock + 1);
                m_bibopTable.registerSmallPageBlocks(
                    reinterpret_cast<uintptr_t>(newPageBlock),
                    firstBlock,
                    size / PAGE_BLOCK_SIZE);

                /// put the new page block to front of the list
                newPageBlock->insertBefore(pageBlock);

                return newPageBlock->allocate();
            }
        }
    }

    /// create new page block
    PageBlock* newPageBlock = createNewPageBlock(size, PAGE_BLOCK_SIZE);
    
    uintptr_t firstBlock = reinterpret_cast<uintptr_t>(newPageBlock + 1);
    m_bibopTable.registerSmallPageBlocks(
        reinterpret_cast<uintptr_t>(newPageBlock),
        firstBlock,
        size / PAGE_BLOCK_SIZE);

    m_pageBlocks.push_back(newPageBlock);
    
    return newPageBlock->allocate();
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

PageBlock* BibopAllocator::createNewPageBlock(uint32_t sizeClass, uint32_t blockSize)
{
    uint8_t* mem = new uint8_t[blockSize];
    uintptr_t pageBlockStart = reinterpret_cast<uintptr_t>(mem + sizeof(PageBlock));

    new(mem) PageBlock(pageBlockStart, sizeClass, blockSize);
    //m_bibopTable.registerSmallPageBlocks(reinterpret_cast<uintptr_t>(mem), pageBlockStart, blockSize / sizeClass);

    return reinterpret_cast<PageBlock*>(mem);
}
