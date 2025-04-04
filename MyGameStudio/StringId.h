#pragma once
#include <cstdint>
#include <array>

struct StringId
{
	uint64_t stringHash;
    std::array<char, 32> stringText;

    constexpr StringId(const uint64_t hash, const char* text) : stringHash(hash), stringText{}
    {
        for (size_t i = 0; i < stringText.size() - 1; i++) {
            stringText[i] = text[i] ? text[i] : '\0';
            if (!text[i]) break;
        }
    }
};

extern constexpr uint32_t MurmurHash3(const char* key, size_t length, uint32_t seed = 0);

static constexpr StringId operator""_sid(const char* stringLiteral, size_t length)
{
	uint64_t stringHash = MurmurHash3(stringLiteral, length);
	return StringId{ stringHash, stringLiteral };
}