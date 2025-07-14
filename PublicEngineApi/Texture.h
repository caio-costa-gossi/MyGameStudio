#pragma once
#include <cstdint>

#include "Err.h"
#include "Image.h"

class Texture
{
private:
	uint32_t textureId_ = 0;
	Image* textureImage_ = nullptr;

	Err InitGlTex();

public:
	Texture();
	Err Init(uint32_t assetId);

	void Use() const;
};
