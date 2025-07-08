#include "ImageProcessor.h"
#include <squish.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "ConsoleManager.h"
#include "DataStream.h"
#include "DdsHeader.h"
#include "LocalizationManager.h"

uint8_t* ImageProcessor::DecompressImageRgba8(const char* filepath, int* x, int* y, int* channels)
{
	return stbi_load(filepath, x, y, channels, 4);
}

uint8_t* ImageProcessor::PadRaw(const uint8_t* src, const uint64_t srcX, const uint64_t srcY, const uint64_t destX, const uint64_t destY)
{
	uint8_t* paddedBuffer = new uint8_t[destX * destY * 4];
	memset(paddedBuffer, 0, destX * destY * 4);

	for (uint64_t y = 0; y < srcY; ++y)
	{
		for (uint64_t x = 0; x < srcX; ++x)
		{
			memcpy_s(&paddedBuffer[(y * destX + x) * 4], 4, &src[(y * srcX + x) * 4], 4);
		}
	}

	return paddedBuffer;
}	

uint8_t* ImageProcessor::Downscale2X(const uint8_t* source, const uint64_t srcX, const uint64_t srcY)
{
	const uint64_t destX = srcX >> 1ULL;
	const uint64_t destY = srcY >> 1ULL;

	const auto destBuffer = new uint8_t[destX * destY * 4];

	for (uint64_t y = 0; y < destY; ++y)
	{
		for (uint64_t x = 0; x < destX; ++x)
		{
			const uint8_t* p1 = &source[(y*2 * srcX + x*2) * 4];
			const uint8_t* p2 = &source[((y*2 + 1) * srcX + x*2) * 4];
			const uint8_t* p3 = &source[(y*2 * srcX + (x*2 + 1)) * 4];
			const uint8_t* p4 = &source[((y*2 + 1) * srcX + (x*2 + 1)) * 4];

			for (int i = 0; i < 4; ++i)
			{
				destBuffer[(y * destX + x) * 4 + i] = 
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

	Mipmap lvl0(0, paddedX, paddedY, static_cast<int>(paddedX * paddedY * 4ULL));
	lvl0.Data = new uint8_t[paddedX * paddedY * 4];
	memcpy_s(lvl0.Data, lvl0.DataSize, paddedBuffer, paddedX * paddedY * 4);
	mipmaps.emplace_back(std::move(lvl0));

	uint8_t curLvl = 1;

	while (paddedX != 1 && paddedY != 1)
	{
		paddedX = paddedX >> 1;
		paddedY = paddedY >> 1;

		Mipmap nextLvl(curLvl, paddedX, paddedY, static_cast<int>(paddedX * paddedY * 4ULL));
		nextLvl.Data = Downscale2X(mipmaps.back().Data, paddedX * 2, paddedY * 2);
		mipmaps.emplace_back(std::move(nextLvl));

		curLvl++;
	}
}

void ImageProcessor::CompressMipmaps(const std::vector<Mipmap>& mipmaps, std::vector<Mipmap>& compressedMipmaps)
{
	for (const Mipmap& m : mipmaps)
	{
		Mipmap compressed(m.Level, m.XSize, m.YSize, squish::GetStorageRequirements(static_cast<int>(m.XSize), static_cast<int>(m.YSize), squish::kDxt5), true);
		squish::CompressImage(m.Data, static_cast<int>(m.XSize), static_cast<int>(m.YSize), compressed.Data, squish::kDxt5);
		compressedMipmaps.emplace_back(std::move(compressed));
	}
}

uint8_t* ImageProcessor::GenerateDdsFile(const std::vector<Mipmap>& compressedMipmaps, const uint64_t originalX, const uint64_t originalY, uint64_t& resultSize)
{
	uint32_t totalBinSize = 0;
	const uint32_t mipmapCount = static_cast<uint32_t>(compressedMipmaps.size());

	for (const Mipmap& m : compressedMipmaps)
	{
		totalBinSize += static_cast<uint32_t>(m.DataSize);
	}

	resultSize = sizeof(DdsHeader) + totalBinSize;
	DataStream stream(resultSize);
	const DdsHeader header(static_cast<uint32_t>(originalY), static_cast<uint32_t>(originalX), static_cast<uint32_t>(originalX * originalY), mipmapCount);

	stream.Write(&header, sizeof(DdsHeader));

	for (const Mipmap& m : compressedMipmaps)
	{
		Err error = stream.Write(m.Data, m.DataSize);

		if (error.Code())
			ConsoleManager::Print("Error writing .dds file! " + error.Message(), enums::ConsoleMessageType::error);
	}

	return stream.Data;
}

uint8_t* ImageProcessor::ProcessImage(const Asset& metadata, uint64_t& resultSize)
{
	int x, y, channels;

	// Load image file in RGBA8
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "0%", enums::ConsoleMessageType::info);
	uint8_t* rgba8 = DecompressImageRgba8(metadata.SourceLocation.c_str(), &x, &y, &channels);
	if (rgba8 == nullptr) return nullptr;

	// Pad image file and delete original buffer
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "20%", enums::ConsoleMessageType::info);
	const uint64_t paddedX = NextPoT(x);
	const uint64_t paddedY = NextPoT(y);
	uint8_t* paddedBuffer = PadRaw(rgba8, x, y, paddedX, paddedY);

	stbi_image_free(rgba8);

	// Generate mipmaps and delete padded buffer
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "50%", enums::ConsoleMessageType::info);
	std::vector<Mipmap> mipmaps;
	GenerateMipmaps(mipmaps, paddedX, paddedY, paddedBuffer);
	delete[] paddedBuffer;

	// Compress mipmaps
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "60%", enums::ConsoleMessageType::info);
	std::vector<Mipmap> compressedMipmaps;
	CompressMipmaps(mipmaps, compressedMipmaps);

	// Generate .tex file
	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "80%", enums::ConsoleMessageType::info);
	uint8_t* finalProduct = GenerateDdsFile(compressedMipmaps, paddedX, paddedY, resultSize);

	ConsoleManager::Print(std::string(LocalizationManager::GetLocalizedString(string_const::G_ASSET_IMPORT)) + "100%", enums::ConsoleMessageType::info);
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
