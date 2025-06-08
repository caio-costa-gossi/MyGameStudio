#include "AssetRuntimeManager.h"
#include "AssetDatabase.h"
#include "ZipFile.h"

Err AssetRuntimeManager::Startup()
{
	return error_const::SUCCESS;
}

uint8_t* AssetRuntimeManager::LoadAsset(const uint32_t assetId)
{
	if (assetData_[assetId].get() != nullptr)
		return assetData_[assetId].get();

	Asset asset;
	Err err = AssetDatabase::GetAsset(assetId, asset);
	if (err.Code())
		return nullptr;

	const ZipFile file(asset.ZipLocation.c_str());
	auto pFileContent = std::make_unique<uint8_t[]>(asset.ProductSize);

	err = file.ReadFile(asset.AssetLocation.c_str(), pFileContent.get(), static_cast<int>(asset.ProductSize));
	if (err.Code())
		return nullptr;

	assetData_[assetId] = std::move(pFileContent);
	return assetData_[assetId].get();
}

auto AssetRuntimeManager::assetData_ = std::unordered_map<uint32_t, std::unique_ptr<uint8_t[]>>();