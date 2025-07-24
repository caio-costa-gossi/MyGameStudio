#pragma once
#include "Asset.h"
#include "Err.h"

class AssetPipeline
{
private:
	static Err GetAssetMetadata(const char* filepath, Asset& asset);
	static uint8_t* ProcessAsset(Asset& assetMetadata, std::string& errMsg, bool ctrlFlag = false);

	static Err SaveResult(Asset& assetMetadata, const uint8_t* assetData);
	static Err SaveFileToZip(const char* zipPath, const char* pathInsideZip, const uint8_t* fileBuffer, uint64_t bufferSize);

	static int64_t LoadFile(const char* filepath, uint8_t* fileBuffer, uint64_t bufferSize);
	static Err SaveFile(const char* filepath, const uint8_t* fileBuffer, uint64_t bufferSize);

	static enums::AssetType GetAssetType(const uint8_t* fileBuffer, uint64_t bufferSize, const std::string& extension);
	static std::string GetTargetExtension(enums::AssetType type);
	static enums::AssetSaveType GetSaveType(enums::AssetType type);

public:
	static Err ImportAsset(const char* filepath, Asset& importedAsset, bool ctrlFlag = false);
};
