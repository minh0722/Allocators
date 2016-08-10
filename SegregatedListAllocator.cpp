#include "SegregatedListAllocator.h"
#include "util.h"
#include <cstdlib>
#include <exception>

SegregatedListAllocator::SegregatedListAllocator(size_t size)
{
    align(size, 2);

    m_mem = malloc(size);

    if (!m_mem)
    {
        throw std::exception("Can't allocate memory");
    }

    m_end = (uint8_t*)m_mem + size;
}


SegregatedListAllocator::~SegregatedListAllocator()
{

}
