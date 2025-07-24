#include "AssetImportationManager.h"
#include "AssetDatabase.h"
#include "AssetPipeline.h"
#include "SystemFileHelper.h"
#include "ZipFile.h"
#include "ConsoleManager.h"
#include "LocalizationManager.h"

#include <vector>

#undef DeleteFile

Err AssetImportationManager::ImportAsset(const char* filepath, const bool ctrlFlag)
{
	Asset newAsset;

	// Find if asset was previously imported. If so, return its ID. Else, return the asset future ID
	Err err = AssetDatabase::FindAssetId(filepath, newAsset.Id);
	if (err.Code())
		return err;

	err = AssetPipeline::ImportAsset(filepath, newAsset, ctrlFlag);
	if (err.Code())
		return err;

	err = AssetDatabase::RegisterAsset(newAsset);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err AssetImportationManager::DeleteAsset(const Asset& asset)
{
	Err err = AssetDatabase::DeleteAsset(asset.Id);
	if (err.Code())
		return err;

	// Asset is outside .zip
	if (asset.ZipLocation.empty())
	{
		err = SystemFileHelper::RemoveFile(asset.AssetLocation.c_str());
		if (err.Code())
			return err;

		return error_const::SUCCESS;
	}

	// Asset is inside .zip
	const ZipFile zip(asset.ZipLocation.c_str());

	if (!zip.IsValid())
		return error_const::GENERIC_EXCEPTION;

	err = zip.DeleteFile(asset.AssetLocation.c_str());
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err AssetImportationManager::ReimportScripts()
{
	ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_RUN_REIMPORT));

	const std::vector<Asset> assets = AssetDatabase::GetAssets();

	for (const Asset& asset : assets)
	{
		if (asset.Type != enums::AssetType::script && asset.Type != enums::AssetType::header)
			continue;

		ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_REIMPORT_PROGRESS) + std::string(" ") + asset.Name);

		Asset newAsset;
		const Err err = AssetPipeline::ImportAsset(asset.SourceLocation.c_str(), newAsset);
		if (err.Code())
			ConsoleManager::PrintError(err);
	}

	return error_const::SUCCESS;
}

