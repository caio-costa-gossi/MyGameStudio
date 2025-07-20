#include "OrthoCamera.h"

Err OrthoCamera::Init(const float leftPlane, const float rightPlane, const float topPlane, const float bottomPlane, const float nearPlane, const float farPlane)
{
	leftPlane_ = leftPlane;
	rightPlane_ = rightPlane;
	topPlane_ = topPlane;
	bottomPlane_ = bottomPlane;

	Camera::farPlane_ = farPlane;
	Camera::nearPlane_ = nearPlane;

	Camera::view_ = glm::lookAt(static_cast<glm::vec3>(Camera::cameraPos_), static_cast<glm::vec3>(Camera::cameraPos_ + Camera::cameraFront_), static_cast<glm::vec3>(Camera::cameraUp_));
	Camera::projection_ = glm::ortho(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);

	return error_const::SUCCESS;
}
