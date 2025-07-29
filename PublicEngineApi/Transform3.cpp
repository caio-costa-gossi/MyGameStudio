#include "Transform3.h"

#include <glm/gtc/type_ptr.hpp>

Transform3::Transform3(const glm::mat3& matrix)
{
	transform_ = matrix;
}

Transform3::Transform3(const glm::mat4& matrix)
{
	transform_ = glm::mat3(matrix);
}

const float* Transform3::GetData() const
{
	return glm::value_ptr(transform_);
}

const glm::mat3& Transform3::GetGlmMatrix() const
{
	return transform_;
}
