#pragma once

#include <iostream>
using namespace std;

struct A
{
    A() { cout << "A constructor " << this << endl; }
    ~A() { cout << "A destructor" << this << endl; }
    int a;
};

struct B
{
    B() { cout << "B constructor " << this << endl; }
    ~B() { cout << "B destructor" << this << endl; }
    int b;
};

namespace
{
    size_t align(const size_t size, const size_t alignment)
    {
        return ((size + alignment - 1) / alignment) * alignment;
    }

    uint32_t alignPowerOfTwo(uint32_t num)
    {
        --num;
        num |= num >> 1;
        num |= num >> 2;
        num |= num >> 4;
        num |= num >> 8;
        num |= num >> 16;
        ++num;

        return num;
    }

    /* 
    @brief align current memory to alignment so that the header size is fitted before it
    @return memory location of header so that the memory after it is aligned to alignment
    */
    size_t alignHeader(const size_t currentMemory, const size_t headerSize, const size_t alignment)
    {
        return align(currentMemory + headerSize, alignment) - headerSize;
    }

    template<uint32_t x>
    struct log2_c
    {
        static const uint32_t value = 1 + log2_c< x / 2>::value;
    };

    template<>
    struct log2_c<1>
    {
        static const uint32_t value = 0;
    };
}
