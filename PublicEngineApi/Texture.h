#pragma once
#include <cstdint>

#include "Err.h"
#include "Image.h"

class Texture
{
private:
	uint32_t textureId_ = 0;
	bool isInit_ = false;

	Image* textureImage_ = nullptr;

	Err InitGlTex();

public:
	Texture() = default;

	// Grab image from AssetRuntimeManager
	Err Init(uint32_t assetId);
	Err Init(const char* filepath, uint32_t& assetId);

	void Use(uint8_t textureChannel) const;
};
