#pragma once

#include <stack>
#include <tiny_gltf.h>

#include "Asset.h"
#include "Err.h"
#include "Mesh.h"
#include "Transform.h"
#include "Vertex.h"

struct MaterialMapsInfo
{
	int32_t DiffuseTexId = -1;
	int32_t DiffuseTexCoordIndex = -1;

	int32_t NormalTexId = -1;
	int32_t NormalTexCoordIndex = -1;

	int32_t MetallicRoughnessTexId = -1;
	int32_t MetallicRoughnessCoordIndex = -1;

	int32_t OcclusionTexId = -1;
	int32_t OcclusionTexCoordIndex = -1;

	int32_t EmissiveTexId = -1;
	int32_t EmissiveTexCoordIndex = -1;

	float MetallicFactor = 0.0f;
	float RoughnessFactor = 0.0f;
	bool DoubleSided = true;
};

struct MeshAuxInfo
{
	Vertex* VertexList = nullptr;
	uint32_t* IndexList = nullptr;

	uint32_t TotalVertexCount = 0;
	uint32_t TotalIndexCount = 0;

	uint32_t ICounter = 0;
	uint32_t VCounter = 0;

	MaterialMapsInfo MapsInfo = { };

	uint32_t MeshIndex = 0;
};

class VertexIndexExtractor
{
private:
	tinygltf::Model model_;

	Mesh* meshList_ = nullptr;
	uint32_t totalMeshCount_ = 0;

	std::stack<Transform> transforms_ = std::stack<Transform>();
	std::unordered_map<int32_t, MeshAuxInfo> meshInfo_ = std::unordered_map<int32_t, MeshAuxInfo>();

	Err CountVerticesIndices();
	Err CountVerticesIndicesNode(const tinygltf::Node& node);
	Err CountMeshes();

	Err ExtractAllVerticesIndices();
	Err ExtractVerticesIndicesNode(const tinygltf::Node& node);

	Err CopyVerticesIndicesBuffer(const tinygltf::Mesh& mesh, const Transform& transform);
	Err ExtractIndices(const tinygltf::Primitive& primitive, MeshAuxInfo& info) const;

	Err StackNodeTransform(const tinygltf::Node& node);
	Err InitMeshes();
	int32_t GetPrimitiveMaterialId(const tinygltf::Primitive& primitive) const;
	Err GetMaterialMapsInfo(int32_t materialId, MaterialMapsInfo& mapsInfo) const;

public:
	explicit VertexIndexExtractor(tinygltf::Model model);

	Err ExtractVerticesIndices(std::unique_ptr<Mesh[]>& meshList, uint32_t& meshCount, const Asset& modelMetadata);;
};
