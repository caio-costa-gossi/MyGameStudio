#pragma once
#include <cstdint>

class ImageLoader
{
public:
	static uint8_t* Load(const char* filepath, int32_t& width, int32_t& height, int32_t& channels);
	static void Free(uint8_t* data);
};
