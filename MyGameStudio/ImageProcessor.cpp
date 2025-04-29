#include "ImageProcessor.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ImageProcessor::ImageProcessor()
{
	
}

uint8_t* ImageProcessor::DecompressImageRgba8(const char* filepath, int* x, int* y, int* channels)
{
	return stbi_load(filepath, x, y, channels, 4);
}

uint8_t* ImageProcessor::ProcessImage(const Asset& metadata)
{
	int x, y, channels;

	// Load image file in RGBA8
	uint8_t* rgba8 = DecompressImageRgba8(metadata.SourceLocation.c_str(), &x, &y, &channels);
	if (rgba8 == nullptr) return nullptr;

	// Pad image file and delete original buffer
	const uint64_t paddedX = NextPoT(x);
	const uint64_t paddedY = NextPoT(y);

	uint8_t* paddedBuffer = new uint8_t[paddedX * paddedY * 4];
	memset(paddedBuffer, 0, paddedX * paddedY * 4);
	memcpy_s(paddedBuffer, paddedX * paddedY * 4, rgba8, metadata.Size);

	stbi_image_free(rgba8);

	// Generate mipmaps and delete padded buffer
	std::vector<Mipmap> mipmaps;
	/*GenerateMipmaps(mipmaps, paddedX, paddedY, paddedBuffer);
	delete[] paddedBuffer;

	// Compress mipmaps
	std::vector<Mipmap> compressedMipmaps;
	CompressMipmaps(mipmaps, compressedMipmaps);*/

	// Generate .tex file
	uint8_t* finalProduct = new uint8_t[1]; // GenerateTexFile();

	return finalProduct;
}

uint64_t ImageProcessor::NextPoT(uint64_t x)
{
	if (x == 0) return 1;

	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	x |= x >> 32;
	x++;

	return x;
}
