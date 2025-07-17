#include "Transform.h"

#include <glm/gtc/type_ptr.hpp>

Transform::Transform(const glm::mat4& matrix)
{
	transform_ = matrix;
}

Transform::Transform(const Vec3F& translation, const float rotationDegrees, const Vec3F& rotationAxis, const Vec3F& scale)
{
	transform_ = glm::translate(transform_, static_cast<glm::vec3>(translation));
	transform_ = glm::rotate(transform_, glm::radians(rotationDegrees), static_cast<glm::vec3>(rotationAxis));
	transform_ = glm::scale(transform_, static_cast<glm::vec3>(scale));
}

void Transform::Translate(const Vec3F& translation)
{
	transform_ = glm::translate(transform_, static_cast<glm::vec3>(translation));
}

void Transform::Rotate(const float rotationDegrees, const Vec3F& rotationAxis)
{
	transform_ = glm::rotate(transform_, glm::radians(rotationDegrees), static_cast<glm::vec3>(rotationAxis));
}

void Transform::Scale(const Vec3F& scale)
{
	transform_ = glm::scale(transform_, static_cast<glm::vec3>(scale));
}

const float* Transform::GetData() const
{
	return glm::value_ptr(transform_);
}
