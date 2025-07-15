#pragma once
#include <cstdint>

#include "Enums.h"
#include "Vertex.h"


struct Mesh
{
	Vertex* VertexList;
	uint32_t VertexCount;
	uint32_t* IndexList;
	uint32_t IndexCount;
	uint32_t TextureAssetId;
	enums::TextureWrapBehavior HorizontalWrap = enums::repeat;
	enums::TextureWrapBehavior VerticalWrap = enums::repeat;
};
