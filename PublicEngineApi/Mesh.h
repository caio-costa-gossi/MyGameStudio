#pragma once
#include <cstdint>
#include <memory>

#include "Enums.h"
#include "Vertex.h"


struct Mesh
{
	uint32_t MeshId = 0;
	std::unique_ptr<Vertex[]> VertexList = nullptr;
	uint32_t VertexCount = 0;
	std::unique_ptr<uint32_t[]> IndexList = nullptr;
	uint32_t IndexCount = 0;
	uint32_t TextureAssetId = 0;
	enums::TextureWrapBehavior HorizontalWrap = enums::repeat;
	enums::TextureWrapBehavior VerticalWrap = enums::repeat;
};
