#pragma once

#include <wrl.h>
#include <windows.h>
#include <sal.h>
#include <cstdint>

#if defined _WIN64
#define ALIGNMENT 8
#elif defined _WIN32
#define ALIGNMENT 16
#endif

size_t alignTo(const size_t size, const size_t alignment);