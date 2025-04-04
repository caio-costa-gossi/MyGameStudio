#include "StringId.h"

constexpr uint32_t MurmurHash3(const char* key, size_t length, uint32_t seed)
{
    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const uint32_t r1 = 15;
    const uint32_t r2 = 13;
    const uint32_t m = 5;
    const uint32_t n = 0xe6546b64;

    uint32_t hash = seed;

    for (size_t i = 0; i < length; i++) {
        uint32_t k = static_cast<uint8_t>(key[i]);
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        hash ^= k;
        hash = (hash << r2) | (hash >> (32 - r2));
        hash = hash * m + n;
    }

    return hash;
}