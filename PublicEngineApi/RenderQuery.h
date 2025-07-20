#pragma once
#include "Camera.h"
#include "MeshInstance.h"
#include "Transform.h"

struct RenderQuery
{
	MeshInstance MeshInstance;
	Transform Model;
	const Camera* Camera = nullptr;
};
