#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform3
{
private:
	glm::mat3 transform_ = glm::mat3(1.0f);

public:
	Transform3() = default;
	Transform3(const glm::mat3& matrix);
	Transform3(const glm::mat4& matrix);

	const float* GetData() const;
	const glm::mat3& GetGlmMatrix() const;
};
