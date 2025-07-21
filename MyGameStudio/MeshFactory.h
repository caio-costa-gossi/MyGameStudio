#pragma once
#include <memory>
#include <tiny_gltf.h>

#include "Err.h"
#include "Mesh.h"

class MeshFactory
{
private:
	static Err GetVertices(const tinygltf::Model& model, const tinygltf::Primitive& primitive, std::unique_ptr<Vertex[]>& vertices, uint32_t& count);

public:
	static Mesh CreateMesh(const tinygltf::Model& model);
};
