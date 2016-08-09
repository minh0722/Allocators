#include "FrameAllocator.h"
#include <cstdlib>
#include <exception>
#include <cstdint>
#include "util.h"

template <typename T>
void callDestructor(void* ptr)
{
    static_cast<T*>(ptr)->~T();
}

FrameAllocator::FrameAllocator(size_t size)
{
    void* mem = malloc(size);

    if (!mem)
        throw std::exception("Can't allocate memory");
    
    m_start = mem;
    m_end = (uint8_t*)mem + size;
    m_current = m_start;
    m_deleters = nullptr;
}

FrameAllocator::~FrameAllocator()
{
    free(m_start);
}

void* FrameAllocator::allocate(size_t size, size_t alignment)
{
    uintptr_t alignedMem = align((uintptr_t)m_current, alignment);

    if (alignedMem + size > (uintptr_t)m_end)
        return nullptr;

    m_current = (uint8_t*)alignedMem + size;

    return (void*)alignedMem;
}

template <typename T>
T* FrameAllocator::allocateObject(size_t alignment)
{
    // allocate memory for deleter and object
    uintptr_t mem = align((uintptr_t)m_current + sizeof(Deleter), alignment);

    if (mem + sizeof(Deleter) + sizeof(T) > (uintptr_t)m_end)
    {
        return nullptr;
    }

    Deleter* deleter = (Deleter*)mem;
    deleter->cleanup = callDestructor<T>;
    deleter->nextDeleter = nullptr;

    if (!m_deleters)
    {
        m_deleters = deleter;
    }
    else
    {
        m_deleters->nextDeleter = deleter;
    }
    
    T* object = (T*)(mem + sizeof(Deleter));
    m_current = object + 1;

    return object;
}

void FrameAllocator::reset()
{
    if (m_deleters)
    {
        do
        {
            m_deleters->cleanup(m_deleters + 1);
            m_deleters = m_deleters->nextDeleter;
        } while (m_deleters);
    }

    m_current = m_start;
}


template A* FrameAllocator::allocateObject<A>(size_t alignment);
template B* FrameAllocator::allocateObject<B>(size_t alignment);