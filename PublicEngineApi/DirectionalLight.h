#pragma once
#include "LightSource.h"

class DirectionalLight : public LightSource
{
private:
	Vec3F direction_ = { };
	float intensity_ = 0.0f;

public:
	DirectionalLight();
	DirectionalLight(Vec3F pos, const ColorRgb& color, Vec3F direction, float intensity = 1.0f);

	Err SetLightUniforms(const Shader& shader) override;

	const Vec3F& GetDirection() const;
	float GetIntensity() const;

	Err SetDirection(const Vec3F& direction);
	Err SetIntensity(float intensity);
};