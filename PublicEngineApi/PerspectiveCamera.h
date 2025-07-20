#pragma once
#include "Camera.h"
#include "Err.h"

class PerspectiveCamera : public Camera
{
private:
	float width_ = 0;
	float height_ = 0;
	float fovRadians_ = 0;

public:
	PerspectiveCamera() = default;

	Err Init(float width = 800.0f, float height = 600.0f, float fovDegrees = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
};
