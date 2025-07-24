#pragma once

#include <stack>
#include <tiny_gltf.h>

#include "Err.h"
#include "Transform.h"
#include "Vertex.h"

class VertexIndexExtractor
{
private:
	tinygltf::Model model_;
	Vertex* vertexList_ = nullptr;
	uint32_t* indexList_ = nullptr;

	std::stack<Transform> transforms_ = std::stack<Transform>();;

	uint32_t iCounter_ = 0;
	uint32_t vCounter_ = 0;
	uint32_t totalVertexCount_ = 0;
	uint32_t totalIndexCount_ = 0;
	
	Err CountVerticesIndices();
	Err CountVerticesIndicesNode(const tinygltf::Node& node);

	Err ExtractAllVerticesIndices();
	Err ExtractVerticesIndicesNode(const tinygltf::Node& node);

	Err CopyVerticesIndicesBuffer(const tinygltf::Mesh& mesh, const Transform& transform);
	Err ExtractIndices(const tinygltf::Primitive& primitive);

	Err StackNodeTransform(const tinygltf::Node& node);

public:
	explicit VertexIndexExtractor(tinygltf::Model model);

	Err ExtractVerticesIndices(std::unique_ptr<Vertex[]>& vertices, uint32_t& vertexCount, std::unique_ptr<uint32_t[]>& indices, uint32_t& indexCount);
};
