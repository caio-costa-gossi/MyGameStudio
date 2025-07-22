#include "AssetRuntimeManager.h"
#include "AssetDatabase.h"
#include "GameConsoleManager.h"
#include "ImageLoader.h"
#include "ZipFile.h"


Err AssetRuntimeManager::Startup()
{
	return error_const::SUCCESS;
}

uint8_t* AssetRuntimeManager::LoadAsset(const uint32_t assetId, uint64_t& assetSize)
{
	if (assetData_[assetId].get() != nullptr)
		return assetData_[assetId].get();

	Asset asset;
	Err err = AssetDatabase::GetAsset(assetId, asset);
	if (err.Code())
		return nullptr;

	assetSize = asset.ProductSize;

	const ZipFile file(asset.ZipLocation.c_str());
	auto pFileContent = std::make_unique<uint8_t[]>(asset.ProductSize);

	err = file.ReadFile(asset.AssetLocation.c_str(), pFileContent.get(), static_cast<int>(asset.ProductSize));
	if (err.Code())
		return nullptr;

	assetData_[assetId] = std::move(pFileContent);
	return assetData_[assetId].get();
}

Image* AssetRuntimeManager::LoadImg(const uint32_t assetId)
{
	if (imageData_.find(assetId) != imageData_.end())
		return &imageData_[assetId];

	// Load asset metadata
	Asset asset;
	Err err = AssetDatabase::GetAsset(assetId, asset);
	if (err.Code())
	{
		GameConsoleManager::PrintError("AssetID " + std::to_string(assetId) + " could not be loaded. Reason: " + err.Message(), enums::ConsoleMessageSender::asset);
		return nullptr;
	}

	// Load compressed image data from zip
	const ZipFile file(asset.ZipLocation.c_str());
	uint8_t* compressedBuffer = new uint8_t[asset.ProductSize];

	err = file.ReadFile(asset.AssetLocation.c_str(), compressedBuffer, static_cast<int>(asset.ProductSize));
	if (err.Code())
	{
		GameConsoleManager::PrintError("Image '" + asset.Name + "' could not be loaded. Reason: " + err.Message(), enums::ConsoleMessageSender::asset);
		delete[] compressedBuffer;
		return nullptr;
	}

	// Decompress with stbi_image and save to imageData_
	imageData_[assetId] = Image(compressedBuffer, static_cast<int32_t>(asset.ProductSize));

	// Deallocate compressed data
	delete[] compressedBuffer;

	return &imageData_[assetId];
}


auto AssetRuntimeManager::assetData_ = AssetMap();
auto AssetRuntimeManager::imageData_ = ImageMap();