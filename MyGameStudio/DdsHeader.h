#pragma once
#include <cstdint>

struct DdsHeader
{
	char MagicNumber[4] = {'D', 'D', 'S', ' '};
	uint32_t Size = 124;
	uint32_t Flags = 0xA1007;
	uint32_t Height;
	uint32_t Width;
	uint32_t PitchOrLinearSize;
	uint32_t Depth = 0;
	uint32_t MipmapCount;
	uint32_t Reserved1[11] = { 0 };

	struct
	{
		uint32_t PixelSize = 32;
		uint32_t PixelFlags = 0x4;
		char FourCc[4] = {'D', 'X', 'T', '5'};
		uint32_t RgbBitCount = 0;
		uint32_t RBitMark = 0;
		uint32_t GBitMask = 0;
		uint32_t BBitMask = 0;
		uint32_t ABitMask = 0;
	} PixelFormat;

	uint32_t Caps = 0x401008;
	uint32_t Caps2 = 0;
	uint32_t Caps3 = 0;
	uint32_t Caps4 = 0;
	uint32_t Reserved2 = 0;

	DdsHeader(const uint32_t height, const uint32_t width, const uint32_t size, const uint32_t mipmapCount) :
		Height(height),
		Width(width),
		PitchOrLinearSize(size),
		MipmapCount(mipmapCount)
	{
		
	}
};
