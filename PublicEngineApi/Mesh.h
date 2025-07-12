#pragma once
#include <cstdint>

struct Mesh
{
	float* VertexList;
	uint32_t VertexCount;
	uint32_t* IndexList;
	uint32_t IndexCount;
};
