#pragma once
#include "Model.h"
#include "Transform.h"

struct RenderRequest
{
	const Model* Model = nullptr;
	Transform Transform;
};
