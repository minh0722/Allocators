#pragma once
#include "BibopTable.h"
#include <vector>

/*
    More efficient implementation of segregated list allocator that uses 
    Bibop method (big bag of pages). 
*/

class BibopAllocator
{
public:
    BibopAllocator();
    ~BibopAllocator();
    
    void* allocate(uint32_t size);
    void free(void* pointer);

private:
    PageBlock* createNewPageBlock(uint32_t sizeClass, uint32_t pageBlockSize);

    void* allocateBigBlock(uint32_t size);

    void destroyPageBlock(PageBlock* pageBlock);

    void freePageBLockMemory(PageBlock* pageBlock);
private:
    BibopTable* m_bibopTable;
    std::vector<PageBlock*> m_pageBlocks;
};

