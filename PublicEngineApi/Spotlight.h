#pragma once
#include "LightSource.h"

class Spotlight : public LightSource
{
private:
	Vec3F direction_ = { };
	float intensity_ = 1.0f;
	float innerCutoffCos_ = 30.0f;
	float outerCutoffCos_ = 35.0f;

public:
	Spotlight();
	Spotlight(Vec3F pos, const ColorRgb& color, Vec3F direction, float innerCutoffDegrees, float outerCutoffDegrees, float intensity = 1.0f);

	Err SetLightUniforms(Shader& shader, uint32_t directionalCount, uint32_t pointCount, uint32_t spotCount) override;

	Vec3F& GetDirection();
	float GetIntensity() const;
	float GetInnerCutoffDegrees() const;
	float GetOuterCutoffDegrees() const;

	Err SetDirection(const Vec3F& direction);
	Err SetIntensity(float intensity);
	Err SetInnerCutoff(float innerCutoffDegrees);
	Err SetOuterCutoff(float outerCutoffDegrees);
};