#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MVector.h"
#include "Transform3.h"

class Transform
{
private:
	glm::mat4 transform_ = glm::mat4(1.0f);

public:
	Transform() = default;
	Transform(const glm::mat4& matrix);
	Transform(const Vec3F& translation, float rotationDegrees, const Vec3F& rotationAxis, const Vec3F& scale);
	Transform(const float* matrixVal);
	Transform(const Transform3& transform);

	void Translate(const Vec3F& translation);
	void Rotate(float rotationDegrees, const Vec3F& rotationAxis);
	void Scale(const Vec3F& scale);

	const float* GetData() const;
	const glm::mat4& GetGlmMatrix() const;

	Transform operator*(const Transform& other) const;
	Vec3F operator*(const Vec3F& vector) const;
};
