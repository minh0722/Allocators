#include "BibopTable.h"
#include "PageBlock.h"
#include "util.h"

BibopTable::BibopTable(uintptr_t startingAddress)
{
    m_startingAddress = startingAddress;
}

void BibopTable::registerSmallPageBlocks(void* pageBlockAddress, uintptr_t firstBlock, size_t blocksCount)
{
    registerSmallPageBlocks(reinterpret_cast<uintptr_t>(pageBlockAddress), firstBlock, blocksCount);
}

void BibopTable::registerSmallPageBlocks(uintptr_t pageBlockAddress, uintptr_t firstBlock, size_t blocksCount)
{
    const uint32_t pageSize = 4096;
    const uint32_t pageSizeLog2 = log2_c<pageSize>::value;

    const uint32_t tagStartingIndex = getTagIndex(firstBlock);
    uintptr_t pageBlockStartingDisposition = ((firstBlock - pageBlockAddress - 1) >> pageSizeLog2);     // [0..127]
    
    for (uint32_t i = tagStartingIndex; i < tagStartingIndex + blocksCount; ++i, ++pageBlockStartingDisposition)
    {
        m_bibopTags[i].setTypeDisposition(BibopTag::type::small, static_cast<uint8_t>(pageBlockStartingDisposition));
    }
}

void BibopTable::registerBigPageBlocks(void* pointer, size_t pageBlocksCount)
{
    registerBigPageBlocks(reinterpret_cast<uintptr_t>(pointer), pageBlocksCount);
}

void BibopTable::registerBigPageBlocks(uintptr_t pointer, size_t pageBlocksCount)
{
    const uint32_t tagIndex = getTagIndex(pointer);

    memset(&m_bibopTags[tagIndex], 0x80, pageBlocksCount * sizeof(BibopTag));
}

PageBlock* BibopTable::getPageBlock(const void* pointer)
{
    BibopTag pointerBibopTag = getBibopTag(pointer);
    return getPageBlock(pointer, pointerBibopTag);
}

PageBlock* BibopTable::getPageBlock(const void* pointer, BibopTag pointerBibopTag)
{
    if (pointerBibopTag.getType() == BibopTag::type::large)
    {
        return reinterpret_cast<PageBlock*>(reinterpret_cast<uintptr_t>(pointer) - sizeof(PageBlock));
    }

    const uint32_t pageSize = 4096;
    const uint32_t pageSizeLog2 = log2_c<pageSize>::value;

    uintptr_t pageIndex = reinterpret_cast<uintptr_t>(pointer) >> pageSizeLog2;
    uintptr_t pageBlock = (pageIndex - pointerBibopTag.getDisposition() - 1) << pageSizeLog2;

    return reinterpret_cast<PageBlock*>(pageBlock);
}

BibopTag BibopTable::getBibopTag(const void* pointer)
{
    uint32_t tagIndex = getTagIndex(pointer);
    return m_bibopTags[tagIndex];
}

uint32_t BibopTable::getTagIndex(const void* pointer)
{
    return getTagIndex(reinterpret_cast<uintptr_t>(pointer));
}

uint32_t BibopTable::getTagIndex(const uintptr_t pointer)
{
    // pointer / 4096

    const uint32_t pageSize = 4096;
    const uint32_t pageSizeLog2 = log2_c<pageSize>::value;

    uintptr_t diff = m_startingAddress - pointer - 1;
    uintptr_t result = (diff & ~(pageSize - 1)) >> pageSizeLog2;

    return ~result & 0xfffff;
}
