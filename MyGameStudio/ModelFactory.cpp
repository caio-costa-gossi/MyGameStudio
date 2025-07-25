#include "ModelFactory.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "Asset.h"
#include "AssetDatabase.h"
#include "ConfigManager.h"
#include "ConsoleManager.h"
#include "DataStream.h"
#include "Image.h"
#include "ImageLoader.h"
#include "SystemPathHelper.h"
#include "VertexIndexExtractor.h"
#include "ZipFile.h"

Model ModelFactory::CreateModel(const tinygltf::Model& model, const Asset& modelMetadata)
{
	Model newModel;
	auto extractor = VertexIndexExtractor(model);

	std::unique_ptr<Mesh[]> meshData;
	uint32_t meshCount;

	// Extract all model information from .glb, import all texture images
	Err err = extractor.ExtractVerticesIndices(meshData, meshCount);
	if (err.Code())
		ConsoleManager::PrintError(err);

	// Get future model ID to save it before committing the data to the asset pipeline
	err = AssetDatabase::FindAssetId(modelMetadata.SourceLocation.c_str(), newModel.ModelId);
	if (err.Code())
		ConsoleManager::PrintError("Error getting future model ID: " + err.Message());

	newModel.MeshCount = meshCount;
	newModel.Meshes = std::move(meshData);

	return newModel;
}

Err ModelFactory::GetVertices(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::unique_ptr<Vertex[]>& vertices, uint32_t& count)
{
	if (primitive.attributes.find("POSITION") == primitive.attributes.end())
	{
		ConsoleManager::PrintWarning("POSITION attribute not found. Skipping primitive...");
		return error_const::IMPORT_INVALID_PRIMITIVE;
	}

	const tinygltf::Accessor vertexAccessor = model.accessors[primitive.attributes.at("POSITION")];
	if (vertexAccessor.type != TINYGLTF_TYPE_VEC3 || vertexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
	{
		ConsoleManager::PrintWarning("Component type is not float or data type is not vec3. Skipping primitive...");
		return error_const::IMPORT_INVALID_PRIMITIVE;
	}

	count = static_cast<uint32_t>(vertexAccessor.count);
	vertices = std::unique_ptr<Vertex[]>(new Vertex[vertexAccessor.count]);

	const tinygltf::BufferView vertexBufferView = model.bufferViews[vertexAccessor.bufferView];
	const tinygltf::Buffer vertexBuffer = model.buffers[vertexBufferView.buffer];

	const size_t offset = vertexAccessor.byteOffset + vertexBufferView.byteOffset;
	const size_t stride = vertexBufferView.byteStride ? vertexBufferView.byteStride : 3 * sizeof(float);

	for (uint32_t i = 0; i < vertexAccessor.count; ++i)
	{
		const float* vertexData = reinterpret_cast<const float*>(vertexBuffer.data.data() + offset + stride * i);

		Vertex newVertex;

		newVertex.Color = { 255,255,255,255 };
		newVertex.Pos.X = vertexData[0];
		newVertex.Pos.Y = vertexData[1];
		newVertex.Pos.Z = vertexData[2];

		vertices[i] = newVertex;
	}

	return error_const::SUCCESS;
}

Err ModelFactory::GetTexCoords(const tinygltf::Model& model, const tinygltf::Primitive& primitive, Mesh& mesh)
{
	const tinygltf::Material material = model.materials[primitive.material];
	const tinygltf::TextureInfo textureInfo = material.pbrMetallicRoughness.baseColorTexture;

	const std::string texCoord = "TEXCOORD_" + std::to_string(textureInfo.texCoord);

	if (primitive.attributes.find(texCoord) == primitive.attributes.end())
	{
		ConsoleManager::PrintWarning("Texture coordinates (" + texCoord + ") not found for primitive.");
		return error_const::IMPORT_INVALID_TEXCOORDS;
	}

	const tinygltf::Accessor texCoordAccessor = model.accessors[primitive.attributes.at(texCoord)];

	if (texCoordAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
	{
		ConsoleManager::PrintWarning("Invalid texture coordinates component data type (" + std::to_string(texCoordAccessor.componentType) + "). Skipping texture coordinates importation...");
		return error_const::IMPORT_INVALID_TEXCOORDS;
	}

	if (texCoordAccessor.type != TINYGLTF_TYPE_VEC2)
	{
		ConsoleManager::PrintWarning("Invalid texture coordinates data type (" + std::to_string(texCoordAccessor.type) + "). Skipping texture coordinates importation...");
		return error_const::IMPORT_INVALID_TEXCOORDS;
	}

	const tinygltf::BufferView texCoordBufferView = model.bufferViews[texCoordAccessor.bufferView];
	const tinygltf::Buffer texCoordBuffer = model.buffers[texCoordBufferView.buffer];

	const size_t texCoordCount = texCoordAccessor.count;
	const size_t offset = texCoordAccessor.byteOffset + texCoordBufferView.byteOffset;
	const size_t stride = texCoordBufferView.byteStride ? texCoordBufferView.byteStride : 2 * sizeof(float);

	const uint8_t* data = (texCoordBuffer.data.data() + offset);

	for (size_t vertex = 0; vertex < texCoordCount; ++vertex)
	{
		const float* coordinates = reinterpret_cast<const float*>(&data[vertex * stride]);

		mesh.VertexList[vertex].TexCoord.X = coordinates[0];
		mesh.VertexList[vertex].TexCoord.Y = coordinates[1];
	}

	return error_const::SUCCESS;
}

Err ModelFactory::GetIndices(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::unique_ptr<uint32_t[]>& indices, uint32_t& count)
{
	const tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

	if (indexAccessor.type != TINYGLTF_TYPE_SCALAR)
	{
		ConsoleManager::PrintWarning("Primitive index data type is invalid. Skipping...");
		return error_const::IMPORT_INVALID_PRIMITIVE;
	}

	count = static_cast<uint32_t>(indexAccessor.count);
	indices = std::unique_ptr<uint32_t[]>(new uint32_t[indexAccessor.count]);

	const tinygltf::BufferView indexBufferView = model.bufferViews[indexAccessor.bufferView];
	const tinygltf::Buffer indexBuffer = model.buffers[indexBufferView.buffer];
	const uint32_t byteOffset = static_cast<uint32_t>(indexAccessor.byteOffset + indexBufferView.byteOffset);

	const uint8_t* dataPtr = &indexBuffer.data[byteOffset];

	for (size_t i = 0; i < indexAccessor.count; ++i)
	{
		switch (indexAccessor.componentType)
		{
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			indices[i] = *(dataPtr + i);
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			indices[i] = *(reinterpret_cast<const uint16_t*>(dataPtr + i * sizeof(uint16_t)));
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			indices[i] = *(reinterpret_cast<const uint32_t*>(dataPtr + i * sizeof(uint32_t)));
			break;
		default:
			ConsoleManager::PrintWarning("Unsupported index component type: " + std::to_string(indexAccessor.componentType));
			return {};
		}
	}

	return error_const::SUCCESS;
}

Err ModelFactory::GetTexture(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const Asset& meshMetadata, uint32_t& textureAssetId)
{
	ConsoleManager::PrintInfo("Extracting texture '" + meshMetadata.Name + "_texture.png' from model...");

	const tinygltf::Material material = model.materials[primitive.material];
	const tinygltf::TextureInfo textureInfo = material.pbrMetallicRoughness.baseColorTexture;
	const tinygltf::Texture texture = model.textures[textureInfo.index];
	const tinygltf::Image textureImage = model.images[texture.source];
	const tinygltf::BufferView textureBufferView = model.bufferViews[textureImage.bufferView];
	const tinygltf::Buffer textureBuffer = model.buffers[textureBufferView.buffer];

	if (textureImage.bits != 8)
	{
		ConsoleManager::PrintWarning("Texture bit depth is not supported (" + std::to_string(textureImage.bits) + "). Texture will not be imported.");
		return error_const::IMPORT_INVALID_TEXTURE;
	}

	const size_t imageSize = textureBufferView.byteLength;
	std::string imageType = textureImage.mimeType;
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

	Err err = SaveTexture(textureAssetId, imageStream.Data, imageSize, format, meshMetadata);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err ModelFactory::SaveTexture(uint32_t& textureAssetId, const uint8_t* imageData, const uint64_t imageSize, const enums::ImageFormat format, const Asset& meshMetadata)
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

	const std::string imageName = meshMetadata.Name + "_texture.png";

	const ZipFile file(zipPath.c_str());
	Err err = file.AddFile(pngData, pngSize, imageName.c_str());
	if (err.Code())
		return err;

	// Register new asset
	Asset imageAsset;
	imageAsset.Name = imageName;
	imageAsset.Type = enums::AssetType::image;
	imageAsset.SourceLocation = meshMetadata.SourceLocation + "-texture";
	imageAsset.SourceSize = imageSize;
	imageAsset.AssetLocation = imageName;
	imageAsset.ProductSize = pngSize;
	imageAsset.ZipLocation = "assets/test.zip";
	imageAsset.Extension = "png";

	err = AssetDatabase::RegisterAsset(imageAsset);
	if (err.Code())
		return err;

	textureAssetId = imageAsset.Id;

	return error_const::SUCCESS;
}