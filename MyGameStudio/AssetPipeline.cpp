#include "AssetPipeline.h"
#include <fstream>

Err AssetPipeline::ImportAsset(const char* filepath)
{
	/*const auto fileBuffer = std::make_unique<uint8_t[]>(10);
	Asset newAsset = GetAssetMetadata()*/

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

Asset AssetPipeline::GetAssetMetadata(const char* filepath, uint8_t* fileBuffer, uint64_t bufferSize)
{
	Asset asset;

	asset.Id = 0;
	asset.SourceLocation = filepath;
	asset.Name = GetFileName(filepath);
	asset.Extension = GetFileExtension(asset.Name);

	int64_t fileSize = LoadFile(filepath, fileBuffer, bufferSize);

	if (fileSize < 0)
		return asset;

	asset.Size = fileSize;
	asset.Type = GetAssetType(fileBuffer, bufferSize);

	return asset;
}

std::string AssetPipeline::GetFileName(const std::string& filepath)
{
	return filepath.substr(filepath.find_last_of('/') + 1);
}

std::string AssetPipeline::GetFileExtension(const std::string& filename)
{
	return filename.substr(filename.find_last_of('.') + 1);
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

	return enums::AssetType::plaintext;
}
