#include "LightingManager.h"

#include "AssetRuntimeManager.h"
#include "RenderManager.h"

Err LightingManager::Startup()
{
	// Load internal engine assets for the light sources
	AssetRuntimeManager::LoadImg("internal_engine_assets/bulb.png", bulbAsset_);

	return error_const::SUCCESS;
}

Err LightingManager::DrawLightSources()
{
	for (const std::pair<const uint32_t, LightSource>& source : localLights_)
	{
		BillboardRenderRequest bulbRenderRequest = { {source.second.GetPos(), {1.0f,1.0f}, bulbAsset_} };
		Err err = RenderManager::RequestBillboardRender(bulbRenderRequest);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err LightingManager::Shutdown()
{
	return error_const::SUCCESS;
}

Err LightingManager::SetLightUniforms(const Shader& shader)
{
	shader.Use();

	shader.SetUniform("ambientColor", ambientLightClr_.R, ambientLightClr_.G, ambientLightClr_.B);
	shader.SetUniform("ambientFactor", ambientLightStr_);
	shader.SetUniform("lightPos", localLights_[0].GetPos().X, localLights_[0].GetPos().Y, localLights_[0].GetPos().Z);
	shader.SetUniform("lightColor", localLights_[0].GetColor().R, localLights_[0].GetColor().G, localLights_[0].GetColor().B);
	shader.SetUniform("lightStrength", localLights_[0].GetIntensity());

	return error_const::SUCCESS;
}

Err LightingManager::SetAmbientLight(const float intensity, const ColorRgb& color)
{
	ambientLightStr_ = intensity;
	ambientLightClr_ = color;

	return error_const::SUCCESS;
}

float LightingManager::GetAmbientLightIntensity()
{
	return ambientLightStr_;
}

ColorRgb LightingManager::GetAmbientLightColor()
{
	return ambientLightClr_;
}

Err LightingManager::AddLightSource(const Vec3F& pos, const ColorRgb& color, const float intensity, uint32_t& lightSrcId)
{
	localLights_[lightSrcCounter_++] = LightSource(pos, color, intensity);
	return error_const::SUCCESS;
}


float LightingManager::ambientLightStr_ = 0;
ColorRgb LightingManager::ambientLightClr_ = { 0,0,0 };

std::unordered_map<uint32_t, LightSource> LightingManager::localLights_;
uint32_t LightingManager::lightSrcCounter_ = 0;

uint32_t LightingManager::bulbAsset_ = 0;