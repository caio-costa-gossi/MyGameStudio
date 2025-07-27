#pragma once
#include "Camera.h"
#include "Err.h"

class OrthoCamera : public Camera
{
private:
	float leftPlane_ = 0;
	float rightPlane_ = 0;
	float topPlane_ = 0;
	float bottomPlane_ = 0;

public:
	OrthoCamera() = default;

	Err Init(enums::CameraControlScheme controlScheme = enums::free_cam, float leftPlane = 0.0f, float rightPlane = 800.0f, float topPlane = 0.0f, float bottomPlane = 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
};
