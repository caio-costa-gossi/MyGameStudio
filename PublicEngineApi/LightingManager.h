#pragma once
#include <unordered_map>
#include <vector>

#include "Err.h"
#include "LightSource.h"
#include "MVector.h"
#include "Shader.h"

class LightingManager
{
private:
	static float ambientLightIntensity_;
	static ColorRgb ambientLightColor_;

	static std::unordered_map<uint32_t, std::unique_ptr<LightSource>> lights_;
	static uint32_t lightSrcCounter_;

	static uint32_t directionalAsset_;
	static uint32_t pointAsset_;
	static uint32_t spotlightAsset_;

	static Err GetSourceAttributes(enums::LightType type, uint32_t& iconId);
	static void CountLightTypes(enums::LightType type, int32_t& directionalCount, int32_t& pointCount, int32_t& spotCount);

public:
	static Err Startup();
	static Err DrawLightSources();
	static Err Shutdown();

	static Err SetLightUniforms(const Shader& shader);

	// Ambient light
	static Err SetAmbientLight(float intensity, const ColorRgb& color);
	static float GetAmbientLightIntensity();
	static ColorRgb GetAmbientLightColor();

	// Local lights
	static Err AddDirectionalLight(const Vec3F& pos, const ColorRgb& color, const Vec3F& direction, float intensity, uint32_t& lightSourceId);
	static Err AddPointLight(const Vec3F& pos, const ColorRgb& color, float distance, uint32_t& lightSourceId);
	static Err AddSpotlight(const Vec3F& pos, const ColorRgb& color, const Vec3F& direction, float innerCutoffDegrees, float outerCutoffDegrees, float intensity, uint32_t& lightSourceId);
	static LightSource* GetLightSource(uint32_t lightSourceId);
};
