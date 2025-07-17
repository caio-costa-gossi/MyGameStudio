#pragma once
#include "Mesh.h"
#include "MeshInstance.h"
#include "Transform.h"

struct RenderQuery
{
	MeshInstance MeshInstance;
	Transform Model;
	Transform View;
	Transform Projection;
};
