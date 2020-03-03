#pragma once
#include <cstdint>


namespace klib
{
    // data: the num word data vector
    // num: data length, > 0 for encode, < 0 for decode
    // key: the 4 word key
    void btea(uint32_t* data, int32_t num, const uint32_t key[4]);
}
