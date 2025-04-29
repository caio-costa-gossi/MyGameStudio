#pragma once
#include "Asset.h"
#include "Err.h"

class AssetPipeline
{
public:
	static int64_t LoadFile(const char* filepath, uint8_t* fileBuffer, uint64_t bufferSize);
	static Asset GetAssetMetadata(const char* filepath);
	static Err SaveFileToZip(const char* zipPath, const char* pathInsideZip, const uint8_t* fileBuffer, uint64_t bufferSize);
	static uint8_t* ProcessAsset(Asset& assetMetadata);

	static enums::AssetType GetAssetType(const uint8_t* fileBuffer, uint64_t bufferSize);

public:
	static Err ImportAsset(const char* filepath);
};
