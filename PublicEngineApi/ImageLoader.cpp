#include "ImageLoader.h"
#include <stb_image.h>

constexpr uint8_t DESIRED_CHANNELS = 4;

uint8_t* ImageLoader::Load(const char* filepath, int32_t& width, int32_t& height, int32_t& channels)
{
	//stbi_set_flip_vertically_on_load(true);
	return stbi_load(filepath, &width, &height, &channels, DESIRED_CHANNELS);
}

uint8_t* ImageLoader::LoadFromMemory(const uint8_t* memoryBytes, const int32_t len, int32_t& width, int32_t& height, int32_t& channels)
{
	//stbi_set_flip_vertically_on_load(true);
	return stbi_load_from_memory(memoryBytes, len, &width, &height, &channels, DESIRED_CHANNELS);
}

void ImageLoader::Free(uint8_t* data)
{
	stbi_image_free(data);
}