#pragma once
#include "Err.h"
#include "Vertex.h"
#include "VertexIndexExtractor.h"

class TextureExtractor
{
private:
	static Err ProcessTexCoords(const tinygltf::Model& gltfModel, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, uint32_t primitiveVertexCount, enums::TextureMap textureMap);

	static Err ImportTextureAsset(const tinygltf::Model& gltfModel, int32_t imageId, const Asset& modelMetadata, int32_t& newAssetId);
	static Err SaveTextureAsset(int32_t& textureAssetId, const uint8_t* imageData, uint64_t imageSize, enums::ImageFormat format, const Asset& modelMetadata, uint32_t imageId);

public:
	static Err ProcessPrimitiveTexCoords(const tinygltf::Model& gltfModel, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, uint32_t primitiveVertexCount);
	static Err ProcessTextureImages(const tinygltf::Model& gltfModel, Mesh* meshData, const std::unordered_map<int32_t, MeshAuxInfo>& meshInfo, const Asset& modelMetadata);

};
