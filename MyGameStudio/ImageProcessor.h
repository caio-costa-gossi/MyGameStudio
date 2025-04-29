#pragma once
#include "Asset.h"
#include "Err.h"

struct Mipmap
{
	uint8_t Level;
	uint64_t XSize;
	uint64_t YSize;
	uint8_t* Data;

	explicit Mipmap(const uint8_t level, const uint64_t xSize, const uint64_t ySize, const bool isCompressed = false, const uint64_t memorySize = 0) : Level(level), XSize(xSize), YSize(ySize)
	{
		Data = isCompressed ? new uint8_t[memorySize] : new uint8_t[XSize * YSize * 4];
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

	static uint64_t NextPoT(uint64_t x);

public:
	ImageProcessor();

	uint8_t* ProcessImage(const Asset& metadata);
};
