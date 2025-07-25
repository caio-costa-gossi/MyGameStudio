#include "TextureExtractor.h"

#include <bitset>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "AssetDatabase.h"
#include "ConfigManager.h"
#include "ConsoleManager.h"
#include "DataStream.h"
#include "Image.h"
#include "SystemPathHelper.h"
#include "ZipFile.h"

#define MAX_TEXTURES_ALLOWED 2048

Err TextureExtractor::ProcessPrimitiveTexCoords(const tinygltf::Model& gltfModel, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, const uint32_t primitiveVertexCount)
{
	Err err = ProcessDiffuseCoords(gltfModel, primitive, info, primitiveVertexCount);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TextureExtractor::ProcessDiffuseCoords(const tinygltf::Model& gltfModel, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, const uint32_t primitiveVertexCount)
{
	if (info.MapsInfo.DiffuseTexId < 0)
		return error_const::SUCCESS;

	if (primitive.attributes.find("TEXCOORD_" + std::to_string(info.MapsInfo.DiffuseTexCoordIndex)) == primitive.attributes.end())
	{
		ConsoleManager::PrintWarning("Tex coord attribute not found, but apparently mesh should have diffuse.");
		return error_const::SUCCESS;
	}

	const tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.at("TEXCOORD_" + std::to_string(info.MapsInfo.DiffuseTexCoordIndex))];

	if (accessor.count != primitiveVertexCount)
	{
		ConsoleManager::PrintError("Primitive TexCoord count is different than primitive vertex count. Skipping...");
		return error_const::SUCCESS;
	}

	if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
	{
		ConsoleManager::PrintWarning("Invalid texture coordinates component data type (" + std::to_string(accessor.componentType) + "). Skipping texture coordinates importation...");
		return error_const::SUCCESS;
	}

	if (accessor.type != TINYGLTF_TYPE_VEC2)
	{
		ConsoleManager::PrintWarning("Invalid texture coordinates data type (" + std::to_string(accessor.type) + "). Skipping texture coordinates importation...");
		return error_const::SUCCESS;
	}

	const tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
	const tinygltf::Buffer& buffer = gltfModel.buffers[bufferView.buffer];

	const size_t texCoordCount = accessor.count;
	const size_t offset = accessor.byteOffset + bufferView.byteOffset;
	const size_t stride = bufferView.byteStride ? bufferView.byteStride : 2 * sizeof(float);

	const uint8_t* data = (buffer.data.data() + offset);

	for (size_t vertex = 0; vertex < texCoordCount; ++vertex)
	{
		const float* coordinates = reinterpret_cast<const float*>(&data[vertex * stride]);

		info.VertexList[info.VCounter + vertex].TexCoord.X = coordinates[0];
		info.VertexList[info.VCounter + vertex].TexCoord.Y = coordinates[1];
	}

	return error_const::SUCCESS;
}

Err TextureExtractor::ProcessTextureImages(const tinygltf::Model& gltfModel, Mesh* meshData, const std::unordered_map<int32_t, MeshAuxInfo>& meshInfo, const Asset& modelMetadata)
{
	std::bitset<MAX_TEXTURES_ALLOWED> texturesImported(0);

	for (const std::pair<int32_t, MeshAuxInfo> info : meshInfo)
	{
		// Ignore meshes without materials (MatIndex == -1)
		if (info.first < 0)
			continue;

		if (info.second.MapsInfo.DiffuseTexId < 0)
			continue;

		Mesh& mesh = meshData[info.second.MeshIndex];
		const int32_t imageId = gltfModel.textures[info.second.MapsInfo.DiffuseTexId].source;

		// Check if texture was already imported and then import

		// Diffuse
		if (!texturesImported.test(imageId))
		{
			Err err = ImportTextureAsset(gltfModel, imageId, modelMetadata, mesh.TextureAssetId);
			if (err.Code())
				ConsoleManager::PrintError("Error trying to import diffuse texture asset: " + err.Message());
		}
	}

	return error_const::SUCCESS;
}

Err TextureExtractor::ImportTextureAsset(const tinygltf::Model& gltfModel, const int32_t imageId, const Asset& modelMetadata, uint32_t& newAssetId)
{
	if (imageId < 0)
		return error_const::SUCCESS;

	ConsoleManager::PrintInfo("Extracting texture '" + modelMetadata.Name + "_texture" + std::to_string(imageId) + ".png' from model...");

	const tinygltf::Image& textureImage = gltfModel.images[imageId];
	const tinygltf::BufferView& textureBufferView = gltfModel.bufferViews[textureImage.bufferView];
	const tinygltf::Buffer& textureBuffer = gltfModel.buffers[textureBufferView.buffer];

	if (textureImage.bits != 8)
	{
		ConsoleManager::PrintWarning("Texture bit depth is not supported (" + std::to_string(textureImage.bits) + "). Texture will not be imported.");
		return error_const::IMPORT_INVALID_TEXTURE;
	}

	const size_t imageSize = textureBufferView.byteLength;
	const std::string imageType = textureImage.mimeType;
	enums::ImageFormat format;

	if (imageType == "image/png")
		format = enums::png;
	else if (imageType == "image/jpeg")
		format = enums::jpeg;
	else
	{
		ConsoleManager::PrintWarning("Texture image type unsupported (" + imageType + "). Skipping texture importation...");
		return error_const::IMPORT_INVALID_TEXTURE;
	}

	const size_t pixelSize = textureImage.component;
	const size_t offset = textureBufferView.byteOffset;
	const size_t stride = textureBufferView.byteStride ? textureBufferView.byteStride : sizeof(uint8_t);;

	const uint8_t* imageData = (textureBuffer.data.data() + offset);

	DataStream imageStream(imageSize);
	for (uint64_t byte = 0; byte < imageSize; byte += stride)
		imageStream.Write(&imageData[byte], sizeof(uint8_t));

	Err err = SaveTextureAsset(newAssetId, imageStream.Data, imageSize, format, modelMetadata, imageId);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TextureExtractor::SaveTextureAsset(uint32_t& textureAssetId, const uint8_t* imageData, const uint64_t imageSize, const enums::ImageFormat format, const Asset& modelMetadata, const uint32_t imageId)
{
	const uint8_t* pngData = imageData;
	int32_t pngSize = static_cast<int32_t>(imageSize);

	// Convert JPEG to PNG
	if (format == enums::jpeg)
	{
		const Image jpgRawImage(imageData, static_cast<int32_t>(imageSize));
		pngData = stbi_write_png_to_mem(jpgRawImage.Data, jpgRawImage.Channels, jpgRawImage.Width, jpgRawImage.Height, jpgRawImage.Channels, &pngSize);
	}

	// Save PNG
	std::string assetDir = ConfigManager::GetConfig("asset_dir");
	SystemPathHelper::WinSeparatorToUnix(assetDir);
	const std::string zipPath = SystemPathHelper::RemoveRelativeSlash(assetDir) + SystemPathHelper::GetUnixSeparator() + "test.zip";

	const std::string imageName = modelMetadata.Name + "_texture" + std::to_string(imageId) + ".png";

	const ZipFile file(zipPath.c_str());
	Err err = file.AddFile(pngData, pngSize, imageName.c_str());
	if (err.Code())
		return err;

	// Register new asset
	Asset imageAsset;
	imageAsset.Name = imageName;
	imageAsset.Type = enums::AssetType::image;
	imageAsset.SourceLocation = modelMetadata.SourceLocation + "-texture" + std::to_string(imageId);
	imageAsset.SourceSize = imageSize;
	imageAsset.AssetLocation = imageName;
	imageAsset.ProductSize = pngSize;
	imageAsset.ZipLocation = zipPath;
	imageAsset.Extension = "png";

	err = AssetDatabase::RegisterAsset(imageAsset);
	if (err.Code())
		return err;

	textureAssetId = imageAsset.Id;

	return error_const::SUCCESS;
}
