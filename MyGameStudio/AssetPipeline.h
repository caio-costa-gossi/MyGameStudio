#pragma once
#include "Asset.h"
#include "Err.h"

class AssetPipeline
{
public:
	static int64_t LoadFile(const char* filepath, uint8_t* fileBuffer, uint64_t bufferSize);
	static Asset GetAssetMetadata(const char* filepath);
	static Err SaveFileToZip(const char* zipPath, const char* pathInsideZip, const uint8_t* fileBuffer, uint64_t bufferSize);
	static uint8_t* ProcessAsset(const Asset& assetMetadata);

	static enums::AssetType GetAssetType(const uint8_t* fileBuffer, uint64_t bufferSize);
	static std::string GetFileName(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filename);

public:
	static Err ImportAsset(const char* filepath);
};
