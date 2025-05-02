#include "AssetPipeline.h"
#include <fstream>
#include "AssetDatabase.h"
#include "ImageProcessor.h"
#include "SystemPathHelper.h"
#include "ZipFile.h"

Err AssetPipeline::ImportAsset(const char* filepath)
{
	// Load file and get metadata
	Asset newAsset = GetAssetMetadata(filepath);

	// Load full file and process
	uint64_t resultSize;
	const uint8_t* resultBuffer = ProcessAsset(newAsset, resultSize);

	if (resultBuffer == nullptr)
		return error_const::GENERIC_EXCEPTION;

	// Save result to .zip
	const std::string zipPath = "assets/test.zip";
	newAsset.ZipLocation = zipPath;
	SaveFileToZip(newAsset.ZipLocation.c_str(), newAsset.LocationInZip.c_str(), resultBuffer, resultSize);

	// Register details in database
	Err error = AssetDatabase::RegisterAsset(newAsset);
	if (error.Code())
		return error;

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

	file.read(reinterpret_cast<char*>(fileBuffer), bufferSize);

	return fileSize;
}

Asset AssetPipeline::GetAssetMetadata(const char* filepath)
{
	Asset asset;
	const auto metadataFileBuffer = std::make_unique<uint8_t[]>(10);

	asset.Id = 0;
	asset.SourceLocation = filepath;
	asset.Name = SystemPathHelper::GetFileName(filepath);
	asset.Extension = SystemPathHelper::GetFileExtension(asset.Name);

	int64_t fileSize = LoadFile(filepath, metadataFileBuffer.get(), 10);

	if (fileSize < 0)
	{
		asset.SourceSize = 0;
		asset.Type = enums::AssetType::undefined;
		return asset;
	}

	asset.SourceSize = fileSize;
	asset.Type = GetAssetType(metadataFileBuffer.get(), 10);

	return asset;
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

uint8_t* AssetPipeline::ProcessAsset(Asset& assetMetadata, uint64_t& resultSize)
{
	uint8_t* returnBuffer = nullptr;

	switch (assetMetadata.Type)
	{
	case enums::AssetType::image:
		returnBuffer = ImageProcessor::ProcessImage(assetMetadata, resultSize);
		break;
	case enums::AssetType::mesh3d:
		returnBuffer = new uint8_t[16];
		resultSize = 16;
		break;
	default:
		returnBuffer = new uint8_t[assetMetadata.SourceSize];
		LoadFile(assetMetadata.SourceLocation.c_str(), returnBuffer, assetMetadata.SourceSize);
		resultSize = assetMetadata.SourceSize;
	}

	assetMetadata.LocationInZip = SystemPathHelper::RemoveFileExtension(assetMetadata.Name) + GetTargetExtension(assetMetadata.Type);
	assetMetadata.ProductSize = resultSize;

	return returnBuffer;
}

enums::AssetType AssetPipeline::GetAssetType(const uint8_t* fileBuffer, uint64_t bufferSize)
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

	if (fileBuffer[0] == 0x67 &&
		fileBuffer[1] == 0x6C &&
		fileBuffer[2] == 0x54 &&
		fileBuffer[3] == 0x46)
		return enums::AssetType::mesh3d;

	return enums::AssetType::plaintext;
}

std::string AssetPipeline::GetTargetExtension(const enums::AssetType type)
{
	switch(type)
	{
	case enums::AssetType::image:
		return ".dds";
	case enums::AssetType::audio:
		return ".mp3";
	case enums::AssetType::video:
		return ".mp4";
	case enums::AssetType::mesh3d:
		return ".glb";
	case enums::AssetType::plaintext:
		return "";
	default:
		return "";
	}
}
