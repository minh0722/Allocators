#include "SegregatedListAllocator.h"
#include "util.h"
#include <cstdlib>
#include <exception>

SegregatedListAllocator::SegregatedListAllocator(size_t size)
{
    align(size, 2);

    uint8_t* mem = (uint8_t*)malloc(size);

    if (!mem)
    {
        throw std::exception("Can't allocate memory");
    }

    m_mem = mem;
    m_end = mem + size;

    size_t blockSize = size - sizeof(Header);
    //uint8_t* blockMem = (uint8_t*)m_mem + sizeof(Header);

    Header* header = (Header*)m_mem;
    new(header) Header(blockSize/*, blockMem*/);

    m_segList.push_back(make_pair(blockSize, header));
}


SegregatedListAllocator::~SegregatedListAllocator()
{
    ::free(m_mem);
}

void SegregatedListAllocator::allocate(size_t size)
{
    size = align(size, 2);



}

SegregatedListAllocator::Header::Header()
{
    blockSize = 0;
    //blockMem = nullptr;
    node.setOwner(this);
}

SegregatedListAllocator::Header::Header(size_t size/*, void * mem*/)
{
    blockSize = size;
    //blockMem = mem;
    node.setOwner(this);
}
