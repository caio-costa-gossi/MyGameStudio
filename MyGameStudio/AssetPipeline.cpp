#include "AssetPipeline.h"
#include <fstream>
#include "AssetDatabase.h"
#include "ConfigManager.h"
#include "ImageProcessor.h"
#include "ModelProcessor.h"
#include "SystemPathHelper.h"
#include "UserScriptProcessor.h"
#include "ZipFile.h"

Err AssetPipeline::ImportAsset(const char* filepath, Asset& importedAsset, const bool ctrlFlag)
{
	// Standardize path separator to unix
	std::string stdFilepath = filepath;
	SystemPathHelper::WinSeparatorToUnix(stdFilepath);

	// Load file and get metadata
	Err err = GetAssetMetadata(stdFilepath.c_str(), importedAsset);
	if (importedAsset.SourceSize == 0)
		return error_const::FILE_NOT_FOUND;
	if (err.Code())
		return err;

	// Load full file and process
	std::string errMsg;
	const uint8_t* resultBuffer = ProcessAsset(importedAsset, errMsg, ctrlFlag);

	if (resultBuffer == nullptr)
		return Err(errMsg, error_const::ASSET_IMPORTATION_ERROR_CODE);

	// Save result to .zip
	SaveResult(importedAsset, resultBuffer);

	// Delete result buffer and return
	delete[] resultBuffer;
	return error_const::SUCCESS;
}

int64_t AssetPipeline::LoadFile(const char* filepath, uint8_t* fileBuffer, uint64_t bufferSize)
{
	std::ifstream file(filepath, std::ios::binary);

	if (!file.is_open())
		return -1;

	file.seekg(0, std::ios::end);
	int64_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	file.read(reinterpret_cast<char*>(fileBuffer), static_cast<uint32_t>(bufferSize));

	return fileSize;
}

Err AssetPipeline::SaveFile(const char* filepath, const uint8_t* fileBuffer, const uint64_t bufferSize)
{
	std::ofstream file(filepath, std::ios::binary | std::ios::trunc);

	if (!file.is_open())
		return error_const::ERROR_OPEN_FILE;

	file.write(reinterpret_cast<const char*>(fileBuffer), static_cast<uint32_t>(bufferSize));
	file.close();

	return error_const::SUCCESS;
}

Err AssetPipeline::GetAssetMetadata(const char* filepath, Asset& asset)
{
	const auto metadataFileBuffer = std::make_unique<uint8_t[]>(10);

	asset.SourceLocation = filepath;
	asset.Name = SystemPathHelper::GetFileName(filepath);
	asset.Extension = SystemPathHelper::GetFileExtension(asset.Name);

	const int64_t fileSize = LoadFile(filepath, metadataFileBuffer.get(), 10);

	if (fileSize < 0)
		return error_const::FILE_NOT_FOUND;

	asset.SourceSize = fileSize;
	asset.Type = GetAssetType(metadataFileBuffer.get(), 10, asset.Extension);

	return error_const::SUCCESS;
}

Err AssetPipeline::SaveResult(Asset& assetMetadata, const uint8_t* assetData)
{
	if (GetSaveType(assetMetadata.Type) == enums::AssetSaveType::save_to_zip)
	{
		// Save to .zip
		assetMetadata.AssetLocation = SystemPathHelper::RemoveFileExtension(assetMetadata.Name) + GetTargetExtension(assetMetadata.Type);

		std::string assetDir = ConfigManager::GetConfig("asset_dir");
		SystemPathHelper::WinSeparatorToUnix(assetDir);

		const std::string zipPath = SystemPathHelper::RemoveRelativeSlash(assetDir) + SystemPathHelper::GetUnixSeparator() + "test.zip";
		assetMetadata.ZipLocation = zipPath;

		SaveFileToZip(assetMetadata.ZipLocation.c_str(), assetMetadata.AssetLocation.c_str(), assetData, assetMetadata.ProductSize);
	}
	else if (GetSaveType(assetMetadata.Type) == enums::AssetSaveType::save_to_assets)
	{
		// Save to assets folder
		std::string assetDir = ConfigManager::GetConfig("asset_dir");
		SystemPathHelper::WinSeparatorToUnix(assetDir);
		assetMetadata.AssetLocation = SystemPathHelper::RemoveRelativeSlash(assetDir) + SystemPathHelper::GetUnixSeparator() + SystemPathHelper::RemoveFileExtension(assetMetadata.Name) + GetTargetExtension(assetMetadata.Type);
		assetMetadata.ZipLocation = "";

		Err err = SaveFile(assetMetadata.AssetLocation.c_str(), assetData, assetMetadata.ProductSize);
		if (err.Code())
			return err;
	}
	else
	{
		// Save to game folder
		std::string gameDir = ConfigManager::GetConfig("game_source_dir");
		SystemPathHelper::WinSeparatorToUnix(gameDir);
		assetMetadata.AssetLocation = SystemPathHelper::RemoveRelativeSlash(gameDir) + SystemPathHelper::GetUnixSeparator() + SystemPathHelper::RemoveFileExtension(assetMetadata.Name) + GetTargetExtension(assetMetadata.Type);
		assetMetadata.ZipLocation = "";

		Err err = SaveFile(assetMetadata.AssetLocation.c_str(), assetData, assetMetadata.ProductSize);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err AssetPipeline::SaveFileToZip(const char* zipPath, const char* pathInsideZip, const uint8_t* fileBuffer, uint64_t bufferSize)
{
	const ZipFile zip(zipPath);

	if (!zip.IsValid())
		return error_const::GENERIC_EXCEPTION;

	Err err = zip.AddFile(fileBuffer, bufferSize, pathInsideZip);

	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

uint8_t* AssetPipeline::ProcessAsset(Asset& assetMetadata, std::string& errMsg, const bool ctrlFlag)
{
	uint8_t* returnBuffer = nullptr;
	uint64_t resultSize;

	switch (assetMetadata.Type)
	{
	case enums::AssetType::image:
		returnBuffer = ImageProcessor::MinimalProcessing(assetMetadata, resultSize);
		break;
	case enums::AssetType::mesh3d_glb:
	case enums::AssetType::mesh3d_gltf:
		returnBuffer = ModelProcessor::ProcessModel(assetMetadata, resultSize, errMsg);
		break;
	case enums::AssetType::script:
		returnBuffer = UserScriptProcessor::ProcessScript(assetMetadata, resultSize, ctrlFlag);
		break;
	case enums::AssetType::header:
		returnBuffer = UserScriptProcessor::ProcessHeader(assetMetadata, resultSize);
		break;
	default:
		returnBuffer = new uint8_t[assetMetadata.SourceSize];
		LoadFile(assetMetadata.SourceLocation.c_str(), returnBuffer, assetMetadata.SourceSize);
		resultSize = assetMetadata.SourceSize;
	}

	assetMetadata.ProductSize = resultSize;

	return returnBuffer;
}

enums::AssetType AssetPipeline::GetAssetType(const uint8_t* fileBuffer, uint64_t bufferSize, const std::string& extension)
{
	if (fileBuffer == nullptr || bufferSize < 8)
		return enums::AssetType::undefined;

	if ((fileBuffer[0] == 0x49 && fileBuffer[1] == 0x44 && fileBuffer[2] == 0x33) ||
		(fileBuffer[0] == 0xFF && (fileBuffer[1] & 0xE0) == 0xE0))
		return enums::AssetType::audio;

	if (fileBuffer[4] == 0x66 && 
		fileBuffer[5] == 0x74 && 
		fileBuffer[6] == 0x79 && 
		fileBuffer[7] == 0x70)
		return enums::AssetType::video;

	if (fileBuffer[0] == 0x89 &&
		fileBuffer[1] == 0x50 &&
		fileBuffer[2] == 0x4E &&
		fileBuffer[3] == 0x47 &&
		fileBuffer[4] == 0x0D &&
		fileBuffer[5] == 0x0A &&
		fileBuffer[6] == 0x1A &&
		fileBuffer[7] == 0x0A)
		return enums::AssetType::image;

	if (extension == "glb")
		return enums::AssetType::mesh3d_glb;
		
	if (extension == "gltf")
		return enums::AssetType::mesh3d_gltf;

	if (extension == "cpp" || extension == "c")
		return enums::AssetType::script;

	if (extension == "h")
		return enums::AssetType::header;

	return enums::AssetType::plaintext;
}

std::string AssetPipeline::GetTargetExtension(const enums::AssetType type)
{
	switch(type)
	{
	case enums::AssetType::image:
		return ".png";
	case enums::AssetType::audio:
		return ".mp3";
	case enums::AssetType::video:
		return ".mp4";
	case enums::AssetType::mesh3d_glb:
	case enums::AssetType::mesh3d_gltf:
		return ".mesh";
	case enums::AssetType::script:
		return ".cpp";
	case enums::AssetType::header:
		return ".h";
	case enums::AssetType::plaintext:
		return "";
	default:
		return "";
	}
}

enums::AssetSaveType AssetPipeline::GetSaveType(enums::AssetType type)
{
	switch (type)
	{
	case enums::AssetType::script:
		return enums::AssetSaveType::save_to_game;
	case enums::AssetType::header:
		return enums::AssetSaveType::save_to_game;
	default:
		return enums::AssetSaveType::save_to_zip;
	}
}
