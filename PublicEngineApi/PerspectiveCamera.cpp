#include "PerspectiveCamera.h"

Err PerspectiveCamera::Init(const float width, const float height, const float fovDegrees, const float nearPlane, const float farPlane)
{
	width_ = width;
	height_ = height;
	fovRadians_ = glm::radians(fovDegrees);

	Camera::nearPlane_ = nearPlane;
	Camera::farPlane_ = farPlane;

	Camera::view_ = glm::lookAt(static_cast<glm::vec3>(Camera::cameraPos_), static_cast<glm::vec3>(Camera::cameraPos_ + Camera::cameraFront_), static_cast<glm::vec3>(Camera::cameraUp_));
	Camera::projection_ = glm::perspective(fovRadians_, width_ / height_, nearPlane_, farPlane_);

	return error_const::SUCCESS;
}
