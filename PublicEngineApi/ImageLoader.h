#pragma once
#include <cstdint>

class ImageLoader
{
public:
	static uint8_t* Load(const char* filepath, int32_t& width, int32_t& height, int32_t& channels);
	static uint8_t* LoadFromMemory(const uint8_t* memoryBytes, int32_t len, int32_t& width, int32_t& height, int32_t& channels, int32_t desiredChannels = 4);
	static void Free(uint8_t* data);
};
