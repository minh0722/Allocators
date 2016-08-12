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

    /* 
    @brief align current memory to alignment so that the header size is fitted before it
    @return memory location of header so that the memory after it is aligned to alignment
    */
    size_t alignHeader(const size_t currentMemory, const size_t headerSize, const size_t alignment)
    {
        return align(currentMemory + headerSize, alignment) - headerSize;
    }
}
