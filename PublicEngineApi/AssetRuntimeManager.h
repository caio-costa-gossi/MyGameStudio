#pragma once
#include <stb_image.h>
#include <unordered_map>

#include "Err.h"
#include "Image.h"

using AssetMap = std::unordered_map<uint32_t, std::unique_ptr<uint8_t[]>>;
using ImageMap = std::unordered_map<uint32_t, Image>;

class AssetRuntimeManager
{
private:
	static AssetMap assetData_;
	static ImageMap imageData_;

public:
	static Err Startup();

	static uint8_t* LoadAsset(uint32_t assetId, uint64_t& assetSize);

	static Image* LoadImg(uint32_t assetId);
	static Image* LoadImg(const char* filepath, uint32_t& assetId);
	static Err FreeImg(uint32_t assetId);
};
