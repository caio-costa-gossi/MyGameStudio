#pragma once
#include "Mesh.h"

struct MeshInstance
{
	const Mesh* Data = nullptr;
	uint32_t ArrayObjectId = 0;
};
