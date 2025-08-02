#pragma once
#include "Mesh.h"
#include "Transform.h"

struct RenderQuery
{
	// Regular tridimensional rendering
	const Mesh* Data = nullptr;
	uint32_t ArrayObjectId = 0;
	Transform Model;
};
