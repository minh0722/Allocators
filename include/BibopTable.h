#pragma once
#include <cstdint>
#include <cassert>

class PageBlock;

struct BibopTag
{
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
        return static_cast<type>(m_TypeDisposition & (1 << 7));
    }

    uint8_t getDisposition() const
    {
        return m_TypeDisposition & 0x7f;
    }

    /// encoding of
    /// page block type - 1 bit
    /// offset of the page - 7 bits
    uint8_t m_TypeDisposition;
};

class BibopTable
{
public:
    void registerSmallPageBlocks(uintptr_t pageBlockHeader, uintptr_t pageBlock, size_t pageBlocksCount);
    
    void registerBigPageBlocks(uintptr_t pageBlock, size_t pageBlocksCount);

    PageBlock* getPageBlock(const void* pointer);

private:
    BibopTag getBibopTag(const void* pointer);

    PageBlock* getPageBlock(const void* pointer, BibopTag pageBlockTagOfPointer);

    uint32_t getTagIndex(const void* pointer);

    uint32_t getTagIndex(const uintptr_t pointer);

private:
    BibopTag m_BibopTags[1024 * 1024];
    uintptr_t m_tableStartingAddress;
};

