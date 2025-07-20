#include "CameraFactory.h"

#include "OrthoCamera.h"
#include "PerspectiveCamera.h"

std::unique_ptr<Camera> CameraFactory::CreateOrtho(const float leftPlane, const float rightPlane, const float topPlane, const float bottomPlane, const float nearPlane, const float farPlane)
{
	auto newCamera = std::make_unique<OrthoCamera>();
	newCamera->Init(leftPlane, rightPlane, topPlane, bottomPlane, nearPlane, farPlane);

	return newCamera;
}

std::unique_ptr<Camera> CameraFactory::CreatePerspective(const float width, const float height, const float fovDegrees, const float nearPlane, const float farPlane)
{
	auto newCamera = std::make_unique<PerspectiveCamera>();
	newCamera->Init(width, height, fovDegrees, nearPlane, farPlane);

	return newCamera;
}
