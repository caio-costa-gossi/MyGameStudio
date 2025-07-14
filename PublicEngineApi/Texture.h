#pragma once
#include <cstdint>

#include "Err.h"

class Texture
{
private:
	int32_t textureId_ = -1;
	uint32_t assetId_ = -1;

public:
	Texture();
	Err Init(const char* filename);

	void Use() const;
};
