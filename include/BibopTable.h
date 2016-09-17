#pragma once
#include <cstdint>
#include <cassert>

class PageBlock;

struct BibopTag
{
    BibopTag() : m_TypeDisposition(0) {}

    enum type
    {
        small = 0,
        large = 1
    };

    void setTypeDisposition(type t, uint8_t disposition)
    {
        assert(disposition <= (uint8_t)(~(1 << 7)));

        setType(t);
        setDisposition(disposition);
    }

    void setType(type t)
    {
        m_TypeDisposition |= (t << 7);
    }

    void setDisposition(uint8_t d)
    {
        m_TypeDisposition |= (d);
    }

    type getType() const
    {
        return static_cast<type>(m_TypeDisposition >> 7);
    }

    uint8_t getDisposition() const
    {
        return m_TypeDisposition & 0x7f;
    }

    bool isLargePageBlock() const
    {
        return getType() == type::large;
    }

    /// page block type - 1 bit
    /// offset of the page - 7 bits
    uint8_t m_TypeDisposition;
};

class BibopTable
{
public:
    BibopTable(uintptr_t startingAddress);

    void registerSmallPageBlocks(void* pageBlockAddress, uintptr_t firstBlock, size_t blocksCount);

    void registerSmallPageBlocks(uintptr_t pageBlockAddress, uintptr_t firstBlock, size_t blocksCount);
    
    void registerBigPageBlocks(void* pointer, size_t pageBlocksCount);

    void registerBigPageBlocks(uintptr_t pointer, size_t pageBlocksCount);

    PageBlock* getPageBlock(const void* pointer);

    BibopTag getBibopTag(const void* pointer);
private:

    PageBlock* getPageBlock(const void* pointer, BibopTag pageBlockTagOfPointer);

    uint32_t getTagIndex(const void* pointer);

    uint32_t getTagIndex(const uintptr_t pointer);

private:
    BibopTag m_bibopTags[1024 * 1024];
    uintptr_t m_startingAddress;
};

