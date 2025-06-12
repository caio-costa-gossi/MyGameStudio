#include "AssetImportationManager.h"
#include "AssetDatabase.h"
#include "AssetPipeline.h"
#include "SystemFileHelper.h"
#include "ZipFile.h"

Err AssetImportationManager::ImportAsset(const char* filepath, const bool ctrlFlag)
{
	Asset newAsset;

	Err err = AssetPipeline::ImportAsset(filepath, newAsset, ctrlFlag);
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
