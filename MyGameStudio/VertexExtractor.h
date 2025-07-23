#pragma once

#include <stack>
#include <tiny_gltf.h>

#include "Err.h"
#include "Transform.h"
#include "Vertex.h"

class VertexExtractor
{
private:
	static std::stack<Transform> transforms_;
	static uint64_t vCounter_;
	
	static Err CountVertices(const tinygltf::Model& model, uint64_t& vertexCount);
	static Err CountVerticesNode(const tinygltf::Model& model, const tinygltf::Node& node, uint64_t& vertexCount);
	static Err ExtractAllVertices(const tinygltf::Model& model, Vertex* vertexList, uint64_t vertexCount);
	static Err ExtractVerticesNode(const tinygltf::Model& model, const tinygltf::Node& node, Vertex* vertexList, uint64_t vertexCount);

	static Err StackNodeTransform(const tinygltf::Node& node);
	static Err CopyVerticesBuffer(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const Transform& transform, Vertex* vertexList, uint64_t vertexCount);

public:
	static Err ExtractVertices(const tinygltf::Model& model, std::unique_ptr<Vertex[]>& vertices, uint64_t& vertexCount);
};
