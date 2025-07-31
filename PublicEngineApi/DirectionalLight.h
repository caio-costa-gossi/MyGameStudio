#pragma once
#include "LightSource.h"

class DirectionalLight : public LightSource
{
private:
	Vec3F direction_ = { };
	float intensity_ = 1.0f;

public:
	DirectionalLight();
	DirectionalLight(Vec3F pos, const ColorRgb& color, Vec3F direction, float intensity = 1.0f);

	Err SetLightUniforms(const Shader& shader, uint32_t directionalCount, uint32_t pointCount, uint32_t spotCount) override;

	Vec3F& GetDirection();
	float GetIntensity() const;

	Err SetDirection(const Vec3F& direction);
	Err SetIntensity(float intensity);
};