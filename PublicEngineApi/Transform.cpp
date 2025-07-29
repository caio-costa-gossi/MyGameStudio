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

Transform::Transform(const float* matrixVal)
{
	transform_ = glm::make_mat4(matrixVal);
}

Transform::Transform(const Transform3& transform)
{
	transform_ = glm::mat4(transform.GetGlmMatrix());
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

const glm::mat4& Transform::GetGlmMatrix() const
{
	return transform_;
}

Transform Transform::operator*(const Transform& other) const
{
	return { transform_ * other.transform_ };
}

Vec3F Transform::operator*(const Vec3F& vector) const
{
	glm::vec4 result = transform_ * glm::vec4(vector.X, vector.Y, vector.Z, 1.0f);
	return { result.x, result.y, result.z };
}
