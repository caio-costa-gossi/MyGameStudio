#pragma once
#include "BillboardData.h"
#include "MeshInstance.h"
#include "Transform.h"

struct RenderQuery
{
	// Regular tridimensional rendering
	MeshInstance MeshInstance;
	Transform Model;

	// Billboard rendering
	BillboardData BillboardData;
	uint32_t BillboardVao;

	bool IsBillboard = false;
};
