#pragma once
#include "pch.h"

class LinearAllocator
{
public:
	LinearAllocator(_In_ size_t size);
	~LinearAllocator();

private:
	uint8_t* m_start;
	size_t m_currentSize;
	size_t m_size;
};

