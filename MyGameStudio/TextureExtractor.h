#pragma once
#include "Err.h"
#include "Vertex.h"
#include "VertexIndexExtractor.h"

class TextureExtractor
{
private:
	static Err ProcessDiffuseCoords(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, uint32_t primitiveVertexCount);

public:
	static Err ProcessPrimitiveTexCoords(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, uint32_t primitiveVertexCount);
	static Err ProcessTextureImage();

};
