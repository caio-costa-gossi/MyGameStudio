#pragma once
#include "Asset.h"
#include "Err.h"

struct Mipmap
{
	uint8_t Level;
	uint64_t XSize;
	uint64_t YSize;
	uint8_t* Data;
	uint64_t DataSize;

	explicit Mipmap(const uint8_t level, const uint64_t xSize, const uint64_t ySize, const int dataSize, const bool isCompressed = false) : Level(level), XSize(xSize), YSize(ySize), DataSize(dataSize)
	{
		Data = isCompressed ? new uint8_t[dataSize] : nullptr;
	}

	~Mipmap()
	{
		delete[] Data;
	}
};

class ImageProcessor
{
public:
	static uint8_t* DecompressImageRgba8(const char* filepath, int* x, int* y, int* channels);
	static void GenerateMipmaps(std::vector<Mipmap>& mipmaps, uint64_t paddedX, uint64_t paddedY, uint8_t* paddedBuffer);
	static void CompressMipmaps(const std::vector<Mipmap>& mipmaps, std::vector<Mipmap>& compressedMipmaps);
	static uint8_t* GenerateTexFile(const std::vector<Mipmap>& compressedMipmaps);

	static uint64_t NextPoT(uint64_t x);
	static uint8_t* Downscale2X(const uint8_t* source, uint64_t srcX, uint64_t srcY);

public:

	static uint8_t* ProcessImage(const Asset& metadata);
};
