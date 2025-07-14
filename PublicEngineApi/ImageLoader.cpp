#include "ImageLoader.h"
#include <stb_image.h>

constexpr uint8_t DESIRED_CHANNELS = 4;

uint8_t* ImageLoader::Load(const char* filepath, int32_t& width, int32_t& height, int32_t& channels)
{
	return stbi_load(filepath, &width, &height, &channels, DESIRED_CHANNELS);
}

void ImageLoader::Free(uint8_t* data)
{
	stbi_image_free(data);
}