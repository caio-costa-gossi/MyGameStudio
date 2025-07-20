#pragma once
#include "Mesh.h"
#include "Transform.h"

struct RenderRequest
{
	const Mesh* Mesh = nullptr;
	Transform Model;
};
