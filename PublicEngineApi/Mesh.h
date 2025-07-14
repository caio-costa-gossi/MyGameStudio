#pragma once
#include <cstdint>
#include "Vertex.h"

struct Mesh
{
	Vertex* VertexList;
	uint32_t VertexCount;
	uint32_t* IndexList;
	uint32_t IndexCount;
};
