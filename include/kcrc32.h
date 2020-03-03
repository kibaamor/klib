#pragma once
#include <cstddef>
#include <cstdint>


namespace klib
{
    uint32_t calc_crc32(const void* data, size_t size);
}
