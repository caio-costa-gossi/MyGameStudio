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
	uint64_t vCounter_ = 0;
	uint64_t totalVertexCount_ = 0;
	
	Err CountVertices();
	Err CountVerticesNode(const tinygltf::Node& node);

	Err ExtractAllVertices();
	Err ExtractVerticesNode(const tinygltf::Node& node);

	Err ExtractIndices();

	Err StackNodeTransform(const tinygltf::Node& node);
	Err CopyVerticesBuffer(const tinygltf::Mesh& mesh, const Transform& transform);

public:
	explicit VertexIndexExtractor(tinygltf::Model model);

	Err ExtractVertices(std::unique_ptr<Vertex[]>& vertices, uint64_t& vertexCount, std::unique_ptr<uint32_t>& indices, uint64_t& indexCount);
};
