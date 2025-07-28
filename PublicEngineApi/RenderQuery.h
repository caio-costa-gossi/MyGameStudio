#pragma once
#include "MeshInstance.h"
#include "Transform.h"

struct RenderQuery
{
	MeshInstance MeshInstance;
	Transform Model;

	bool IsBillboard = false;
};
