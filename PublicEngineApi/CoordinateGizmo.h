#pragma once
#include <cstdint>

#include "Shader.h"

class CoordinateGizmo
{
private:
	uint32_t vaoId_ = 0;
	uint16_t camId_ = 0;

	void BuildVao();
	void BuildCamera();

public:
	CoordinateGizmo() = default;
	void InitGizmo();
	void Draw(const Shader& shader);
};
