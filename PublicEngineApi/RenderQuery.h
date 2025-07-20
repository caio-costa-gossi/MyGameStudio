#pragma once
#include "Camera.h"
#include "MeshInstance.h"
#include "Transform.h"

struct RenderQuery
{
	MeshInstance MeshInstance;
	Transform Model;
	Camera* Camera = nullptr;
};
