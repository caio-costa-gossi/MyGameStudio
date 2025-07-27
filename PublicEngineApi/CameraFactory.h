#pragma once
#include <memory>

#include "Camera.h"

class CameraFactory
{
private:


public:
	static std::unique_ptr<Camera> CreateOrtho(enums::CameraControlScheme controlScheme = enums::free_cam, float leftPlane = 0.0f, float rightPlane = 800.0f, float topPlane = 0.0f, float bottomPlane = 600.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
	static std::unique_ptr<Camera> CreatePerspective(enums::CameraControlScheme controlScheme = enums::free_cam, float width = 800.0f, float height = 600.0f, float fovDegrees = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f);
};
