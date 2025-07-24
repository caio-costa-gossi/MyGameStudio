#pragma once
#include <memory>
#include <tiny_gltf.h>

#include "Asset.h"
#include "Err.h"
#include "Mesh.h"
#include "Model.h"

class ModelFactory
{
private:
	static Err GetVertices(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::unique_ptr<Vertex[]>& vertices, uint32_t& count);
	static Err GetTexCoords(const tinygltf::Model& model, const tinygltf::Primitive& primitive, Mesh& mesh);
	static Err GetIndices(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::unique_ptr<uint32_t[]>& indices, uint32_t& count);
	static Err GetTexture(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const Asset& meshMetadata, uint32_t& textureAssetId);

	static Err SaveTexture(uint32_t& textureAssetId, const uint8_t* imageData, uint64_t imageSize, enums::ImageFormat format, const Asset& meshMetadata);

public:
	static Model CreateModel(const tinygltf::Model& model, const Asset& modelMetadata);
};
