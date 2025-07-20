#pragma once
#include "Camera.h"
#include "Mesh.h"
#include "Transform.h"

struct RenderRequest
{
	const Mesh* Mesh = nullptr;
	Transform Model;
	Camera* Camera = nullptr;
};
