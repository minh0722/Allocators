#include "BibopTable.h"
#include "util.h"

void BibopTable::registerSmallPageBlocks(uintptr_t pageBlockHeader, uintptr_t pageBlock, size_t pageBlocksCount)
{
    const uint32_t pageSize = 4096;
    const uint32_t pageSizeLog2 = log2_c<pageSize>::value;

    const uint32_t tagStartingIndex = getTagIndex(pageBlock);
    uint8_t pageBlockStartingDisposition = static_cast<uint8_t>((pageBlock - pageBlockHeader) >> pageSizeLog2);     // should be between [0..127]
    
    for (uint32_t i = tagStartingIndex; i < tagStartingIndex + pageBlocksCount; ++i, ++pageBlockStartingDisposition)
    {
        m_BibopTags[i].setTypeDisposition(BibopTag::type::small, pageBlockStartingDisposition);
    }
}

void BibopTable::registerBigPageBlocks(uintptr_t pageBlock, size_t pageBlocksCount)
{
    const uint32_t tagIndex = getTagIndex(pageBlock);

    memset(&m_BibopTags[tagIndex], 0x80, pageBlocksCount * sizeof(BibopTag));
}

PageBlock* BibopTable::getPageBlock(const void* pointer)
{
    BibopTag pointerBibopTag = getBibopTag(pointer);
    return getPageBlock(pointer, pointerBibopTag);
}

PageBlock* BibopTable::getPageBlock(const void* pointer, BibopTag pointerBibopTag)
{
    const uint32_t pageSize = 4096;
    const uint32_t pageSizeLog2 = log2_c<pageSize>::value;

    uintptr_t pageIndex = reinterpret_cast<uintptr_t>(pointer) >> pageSizeLog2;
    uintptr_t pageBlock = (pageIndex - pointerBibopTag.getDisposition()) << pageSizeLog2;

    return reinterpret_cast<PageBlock*>(pageBlock);
}

BibopTag BibopTable::getBibopTag(const void* pointer)
{
    uint32_t tagIndex = getTagIndex(pointer);
    return m_BibopTags[tagIndex];
}

uint32_t BibopTable::getTagIndex(const void* pointer)
{
    return getTagIndex(reinterpret_cast<uintptr_t>(pointer));
}

uint32_t BibopTable::getTagIndex(const uintptr_t pointer)
{
    const uint32_t pageSize = 4096;
    const uint32_t pageSizeLog2 = log2_c<pageSize>::value;

    uintptr_t diff = m_tableStartingAddress - pointer;
    uintptr_t result = (diff & ~(pageSize - 1)) >> pageSizeLog2;

    return ~result & 0xfffff;
}
