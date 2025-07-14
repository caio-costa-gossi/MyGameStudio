#pragma once
#include <cstdint>

struct Image
{
	int32_t Width;
	int32_t Height;
	int32_t Channels;
	uint8_t* Data = nullptr;

	Image() = default;
	explicit Image(const char* filepath);
	explicit Image(const uint8_t* imageBuffer, int32_t bufferLen);
	~Image();
	Image(const Image& other) = delete;
	Image(Image&& other) noexcept;

	Image operator=(const Image& other) = delete;
	Image& operator=(Image&& other) noexcept;
};
