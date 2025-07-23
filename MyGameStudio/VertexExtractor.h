#pragma once

#include <tiny_gltf.h>

#include "Err.h"
#include "Vertex.h"

class VertexExtractor
{
private:
	static uint64_t vCounter_;
	
	static Err CountVertices(const tinygltf::Model& model, uint64_t& vertexCount);
	static Err CountVerticesNode(const tinygltf::Model& model, const tinygltf::Node& node, uint64_t& vertexCount);

public:
	static Err ExtractVertices(const tinygltf::Model& model, std::unique_ptr<Vertex[]>& vertices, uint64_t& vertexCount);
};
