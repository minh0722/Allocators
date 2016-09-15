#include "PageBlock.h"

PageBlock::PageBlock(uintptr_t memory, uint32_t sizeOfEachBlock, size_t memorySize) :
    m_memory(memory), m_sizeOfEachBlocks(sizeOfEachBlock),
    m_memorySize(memorySize),
    m_currentUnallocatedOffset(0), m_currentFreedBlockOffset(0)
{
    m_freedBlocksCount = convertToBlocksCount(memorySize);
}

void* PageBlock::allocate()
{
    void* mem = nullptr;

    if (m_currentFreedBlockOffset != 0)
    {
        /// allocate from freed blocks/holes
        uint32_t offset = convertToBytes(m_currentFreedBlockOffset);
        mem = reinterpret_cast<void*>(m_memory + offset);

        m_currentFreedBlockOffset = *reinterpret_cast<uint16_t*>(mem);
        --m_freedBlocksCount;
    }
    else
    {
        if (m_currentUnallocatedOffset == -1)
        {
            return nullptr;
        }

        /// allocate from contiguous unallocated area
        uint32_t offset = convertToBytes(m_currentUnallocatedOffset);

        mem = reinterpret_cast<void*>(m_memory + offset);

        ++m_currentUnallocatedOffset;

        if (convertToBytes(m_currentUnallocatedOffset) > m_memorySize)
        {
            m_currentUnallocatedOffset = -1;
        }

        --m_freedBlocksCount;
    }

    return mem;
}

void PageBlock::free(void* ptr)
{
    uintptr_t pageStart = m_memory;
    uintptr_t pointer = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t offset = pointer - pageStart;

    /// write the offset of the m_prev freed block
    *reinterpret_cast<uint16_t*>(pointer) = m_currentFreedBlockOffset;
    m_currentFreedBlockOffset = convertToBlocksCount(offset);
    ++m_freedBlocksCount;
}

uint16_t PageBlock::getSizeClass() const
{
    return m_sizeOfEachBlocks;
}

bool PageBlock::hasFreeBlock() const
{
    return m_freedBlocksCount > 0;
}

uint16_t PageBlock::convertToBlocksCount(size_t bytes)
{
    return static_cast<uint16_t>(bytes / m_sizeOfEachBlocks);
}

uint32_t PageBlock::convertToBytes(uint16_t blocks)
{
    return static_cast<uintptr_t>(blocks * m_sizeOfEachBlocks);
}

