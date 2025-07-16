#pragma once
#include "Mesh.h"

struct RenderRequest
{
	const Mesh* Mesh = nullptr;
	Vec3F GlobalPosition = { 0,0,0 };
	float GlobalRotation = 0;
	Vec3F GlobalRotationAxis = { 0,0,0 };
	Vec3F GlobalScale = { 1,1,1 };
};
