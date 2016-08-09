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

struct Deleter
{
    void (*cleanup)(void* ptr);
    Deleter* nextDeleter;
};

class FrameAllocator
{
public:
    FrameAllocator(size_t size);
    ~FrameAllocator();

    void* allocate(size_t size, size_t alignment);

    template <typename T>
    T* allocateObject(size_t alignment);
    
    void reset();
private:
    void* m_start;
    void* m_end;
    void* m_current;

    Deleter* m_deleters;
};