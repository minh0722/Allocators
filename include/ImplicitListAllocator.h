#pragma once
#include "common.h"

struct BlockHeader
{
	// TODO: maybe uint64_t for x64?
	uint32_t size : 31;
	uint8_t isAllocated : 1;
};

class ImplicitListAllocator
{
public:
	ImplicitListAllocator(_In_ size_t size);
	~ImplicitListAllocator();

	/**
	@warning don't use odd alignment
	*/
	void* allocate(_In_ size_t size, _In_opt_ size_t alignment = ALIGNMENT);
private:
	void* m_start;
	size_t m_size;
};