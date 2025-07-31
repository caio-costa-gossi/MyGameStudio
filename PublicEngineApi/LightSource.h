#pragma once
#include "Color.h"
#include "Err.h"
#include "Shader.h"

class LightSource
{
protected:
	Vec3F pos_ = { };
	ColorRgb color_ = { };

	enums::LightType type_ = enums::point;

public:
	LightSource() = default;
	LightSource(Vec3F pos, const ColorRgb& color);

	virtual Err SetLightUniforms(const Shader& shader, uint32_t directionalCount, uint32_t pointCount, uint32_t spotCount) = 0;
	virtual ~LightSource() = default;

	Vec3F& GetPos();
	ColorRgb& GetColor();
	enums::LightType GetType() const;

	Err SetPos(const Vec3F& pos);
	Err SetColor(const ColorRgb& color);
};
