#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MVector.h"

class Transform
{
private:
	glm::mat4 transform_ = glm::mat4(1.0f);

public:
	Transform() = default;
	Transform(const glm::mat4& matrix);
	Transform(const Vec3F& translation, float rotationDegrees, const Vec3F& rotationAxis, const Vec3F& scale);

	void Translate(const Vec3F& translation);
	void Rotate(float rotationDegrees, const Vec3F& rotationAxis);
	void Scale(const Vec3F& scale);

	const float* GetData() const;
};
