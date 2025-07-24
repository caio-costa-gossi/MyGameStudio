#pragma once
#include <memory>

#include "Mesh.h"

struct Model
{
	uint32_t MeshCount = 0;
	std::unique_ptr<Mesh[]> Meshes;
};
