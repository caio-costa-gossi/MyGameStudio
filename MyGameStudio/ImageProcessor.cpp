#include "ImageProcessor.h"
#include <squish.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "ConsoleManager.h"
#include "DataStream.h"

uint8_t* ImageProcessor::DecompressImageRgba8(const char* filepath, int* x, int* y, int* channels)
{
	return stbi_load(filepath, x, y, channels, 4);
}

uint8_t* ImageProcessor::Downscale2X(const uint8_t* source, const uint64_t srcX, const uint64_t srcY)
{
	const uint64_t destX = srcX >> 1ULL;
	const uint64_t destY = srcY >> 1ULL;

	const auto destBuffer = new uint8_t[destX * destY * 4];

	for (uint64_t x = 0; x < destX; ++x)
	{
		for (uint64_t y = 0; y < destY; ++y)
		{
			const uint8_t* p1 = &source[(x * srcY + y) * 4];
			const uint8_t* p2 = &source[((x + 1) * srcY + y) * 4];
			const uint8_t* p3 = &source[(x * srcY + (y + 1)) * 4];
			const uint8_t* p4 = &source[((x + 1) * srcY + (y + 1)) * 4];

			for (int i = 0; i < 4; ++i)
			{
				destBuffer[x * destY + y + i] = 
					(	static_cast<int>(p1[i]) + 
						static_cast<int>(p2[i]) + 
						static_cast<int>(p3[i]) + 
						static_cast<int>(p4[i])) / 4;
			}
		}
	}

	return destBuffer;
}

void ImageProcessor::GenerateMipmaps(std::vector<Mipmap>& mipmaps, uint64_t paddedX, uint64_t paddedY, uint8_t* paddedBuffer)
{
	mipmaps.reserve(8);

	Mipmap lvl0(0, paddedX, paddedY, paddedX * paddedY * 4ULL);
	memcpy_s(lvl0.Data, lvl0.DataSize, paddedBuffer, paddedX * paddedY * 4);
	mipmaps.emplace_back(lvl0);

	uint8_t curLvl = 1;

	while (paddedX != 1 && paddedY != 1)
	{
		paddedX = paddedX >> 1;
		paddedY = paddedY >> 1;

		Mipmap nextLvl(curLvl, paddedX, paddedY, paddedX * paddedY * 4ULL);
		nextLvl.Data = Downscale2X(mipmaps.back().Data, paddedX * 2, paddedY * 2);
		mipmaps.emplace_back(nextLvl);

		curLvl++;
	}
}

void ImageProcessor::CompressMipmaps(const std::vector<Mipmap>& mipmaps, std::vector<Mipmap>& compressedMipmaps)
{
	for (const Mipmap& m : mipmaps)
	{
		Mipmap compressed(m.Level, m.XSize, m.YSize, squish::GetStorageRequirements(m.XSize, m.YSize, squish::kDxt5), true);
		squish::CompressImage(m.Data, m.XSize, m.YSize, compressed.Data, squish::kDxt5);
		compressedMipmaps.emplace_back(compressed);
	}
}

uint8_t* ImageProcessor::GenerateTexFile(const std::vector<Mipmap>& compressedMipmaps, const uint64_t originalX, const uint64_t originalY)
{
	uint64_t totalTexSize = 0;
	const uint64_t mipmapCount = compressedMipmaps.size();

	for (const Mipmap& m : compressedMipmaps)
	{
		totalTexSize += m.DataSize + 3 * sizeof(uint64_t);
	}

	DataStream stream(totalTexSize + 24);
	stream.Write(&originalX, sizeof(uint64_t));
	stream.Write(&originalY, sizeof(uint64_t));
	stream.Write(&mipmapCount, sizeof(uint64_t));

	for (const Mipmap& m : compressedMipmaps)
	{
		stream.Write(&m.XSize, sizeof(uint64_t));
		stream.Write(&m.YSize, sizeof(uint64_t));
		stream.Write(&m.DataSize, sizeof(uint64_t));
		Err error = stream.Write(m.Data, m.DataSize);

		if (error.Code())
			ConsoleManager::Print("Error writing .tex file! " + error.Message(), enums::ConsoleMessageType::error);
	}

	return stream.Data;
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
	GenerateMipmaps(mipmaps, paddedX, paddedY, paddedBuffer);
	delete[] paddedBuffer;

	// Compress mipmaps
	std::vector<Mipmap> compressedMipmaps;
	CompressMipmaps(mipmaps, compressedMipmaps);

	// Generate .tex file
	uint8_t* finalProduct = GenerateTexFile(compressedMipmaps, paddedX, paddedY);
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
