#pragma once

#include <wrl.h>
#include <windows.h>
#include <sal.h>
#include <cstdint>

#if defined _WIN64
#define GRANULARITY 64
#elif defined _WIN32
#define ALIGNMENT 32
#endif

size_t alignTo(const size_t size, const size_t alignment);