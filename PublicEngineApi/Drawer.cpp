#include "Drawer.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "CameraManager.h"
#include "GameConsoleManager.h"
#include "LightingManager.h"
#include "NumericUtils.h"
#include "Transform.h"

Err Drawer::Init()
{
	Err err = InitShaders();
	if (err.Code())
		return err;

	coordGizmo_.InitGizmo(regularShader_, billboardShader_);

	return error_const::SUCCESS;
}

Err Drawer::InitShaders()
{
	GameConsoleManager::PrintInfo("Initializing shaders...", enums::ConsoleMessageSender::render);

	// Standard shader
	Err err = regularShader_.Init("shaders/RegularShader.vert", "shaders/RegularShader.frag");
	if (err.Code())
		return err;

	err = regularShader_.Build();
	if (err.Code())
		return err;

	err = InitUniformIdsRegular();
	if (err.Code())
		return err;

	// Billboard shader
	err = billboardShader_.Init("shaders/BillboardShader.vert", "shaders/BillboardShader.frag");
	if (err.Code())
		return err;

	err = billboardShader_.Build();
	if (err.Code())
		return err;

	err = InitUniformIdsBillboard();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err Drawer::InitUniformIdsRegular()
{
	regularShader_.Use();

	// Aux
	uUseVertexColor_ = regularShader_.GetUniformId("useVertexColor");
	uNormalMatrix_ = regularShader_.GetUniformId("normalMatrix");

	// Transforms
	uModel_ = regularShader_.GetUniformId("model");
	uView_ = regularShader_.GetUniformId("view");
	uProjection_ = regularShader_.GetUniformId("projection");

	// Material textures
	uBaseColorTex_ = regularShader_.GetUniformId("materialProps.baseColorTex");
	uNormalTex_ = regularShader_.GetUniformId("materialProps.normalTex");
	uMetallicRoughtnessTex_ = regularShader_.GetUniformId("materialProps.metallicRoughnessTex");
	uOcclusionTex_ = regularShader_.GetUniformId("materialProps.occlusionTex");
	uEmissiveTex_ = regularShader_.GetUniformId("materialProps.emissiveTex");

	return error_const::SUCCESS;
}

Err Drawer::InitUniformIdsBillboard()
{
	billboardShader_.Use();

	uBboardCenterWorld_ = billboardShader_.GetUniformId("centerWorld");
	uBboardScale_ = billboardShader_.GetUniformId("scale");
	uBboardView_ = billboardShader_.GetUniformId("view");
	uBboardProjection_ = billboardShader_.GetUniformId("projection");

	return error_const::SUCCESS;
}

void Drawer::Draw(std::queue<RenderQuery>& queries, std::priority_queue<BillboardRenderQuery>& billboardQueries, const TextureList& textures)
{
	// Reset color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set lighting sources
	Err err = LightingManager::SetLightUniforms(regularShader_);
	if (err.Code())
		GameConsoleManager::PrintError("Error setting lighting uniforms: " + err.Message(), enums::ConsoleMessageSender::render);

	// Draw 3d meshes with standard shader
	regularShader_.Use();
	SetShaderConfig();
	SetStaticUniformsRegular();

	while (!queries.empty())
	{
		const RenderQuery& query = queries.front();
		DrawRegular(query, textures);
		queries.pop();
	}

	// Draw billboards with billboard shader
	billboardShader_.Use();
	while (!billboardQueries.empty())
	{
		const BillboardRenderQuery& query = billboardQueries.top();
		DrawBillboard(query, textures);
		billboardQueries.pop();
	}

	coordGizmo_.Draw(regularShader_, billboardShader_);
}

void Drawer::DrawRegular(const RenderQuery& query, const TextureList& textures)
{
	// Prepare to draw
	SetShaderUniformsRegular(query);
	SetTextureWrapping(query.MeshInstance);

	// Load textures into their OpenGL units
	if (query.MeshInstance.Data->Material.BaseColorTexture >= 0)
		textures.at(query.MeshInstance.Data->Material.BaseColorTexture).Use(enums::TextureMap::base_color);

	if (query.MeshInstance.Data->Material.NormalTexture >= 0)
		textures.at(query.MeshInstance.Data->Material.NormalTexture).Use(enums::TextureMap::normal);

	if (query.MeshInstance.Data->Material.MetallicRoughnessTexture >= 0)
		textures.at(query.MeshInstance.Data->Material.MetallicRoughnessTexture).Use(enums::TextureMap::metallic_roughness);

	if (query.MeshInstance.Data->Material.OcclusionTexture >= 0)
		textures.at(query.MeshInstance.Data->Material.OcclusionTexture).Use(enums::TextureMap::occlusion);

	if (query.MeshInstance.Data->Material.EmissiveTexture >= 0)
		textures.at(query.MeshInstance.Data->Material.EmissiveTexture).Use(enums::TextureMap::emissive);

	// Draw
	glBindVertexArray(query.MeshInstance.ArrayObjectId);
	glDrawElements(GL_TRIANGLES, static_cast<int32_t>(query.MeshInstance.Data->IndexCount), GL_UNSIGNED_INT, nullptr);
}

void Drawer::DrawBillboard(const BillboardRenderQuery& query, const TextureList& textures)
{
	// Prepare to draw
	SetShaderConfig();
	SetShaderUniformsBillboard(billboardShader_, query);
	textures.at(query.BillboardData.BillboardImageId).Use(0);

	// Draw
	glBindVertexArray(query.BillboardVao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Drawer::SetShaderConfig()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Drawer::SetStaticUniformsRegular()
{
	// Material textures
	Shader::SetUniform(uBaseColorTex_, enums::TextureMap::base_color);
	Shader::SetUniform(uNormalTex_, enums::TextureMap::normal);
	Shader::SetUniform(uMetallicRoughtnessTex_, enums::TextureMap::metallic_roughness);
	Shader::SetUniform(uOcclusionTex_, enums::TextureMap::occlusion);
	Shader::SetUniform(uEmissiveTex_, enums::TextureMap::emissive);
}

void Drawer::SetShaderUniformsRegular(const RenderQuery& query)
{
	Shader::SetUniform(uUseVertexColor_, query.MeshInstance.Data->Material.BaseColorTexture < 0);

	// Pre-calculate normal matrix for model
	Shader::SetUniform(uNormalMatrix_, enums::MatrixDim::m3x3, NumericUtils::CalculateNormalMatrix(query.Model).GetData(), false);

	// Transforms
	Shader::SetUniform(uModel_, enums::MatrixDim::m4x4, query.Model.GetData(), false);
	Shader::SetUniform(uView_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetView().GetData(), false);
	Shader::SetUniform(uProjection_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetProjection().GetData(), false);
}

void Drawer::SetShaderUniformsBillboard(Shader& shader, const BillboardRenderQuery& query)
{
	// Transforms
	Shader::SetUniform(uBboardCenterWorld_, query.BillboardData.WorldPos.X, query.BillboardData.WorldPos.Y, query.BillboardData.WorldPos.Z);
	Shader::SetUniform(uBboardScale_, query.BillboardData.Scale.X, query.BillboardData.Scale.Y);
	Shader::SetUniform(uBboardView_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetView().GetData(), false);
	Shader::SetUniform(uBboardProjection_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetProjection().GetData(), false);
}

void Drawer::SetTextureWrapping(const MeshInstance& mesh)
{
	// S (Horizontal)
	switch (mesh.Data->HorizontalWrap)
	{
	case enums::repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
	case enums::mirror_repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		break;
	case enums::clamp_border:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		break;
	case enums::clamp_edge:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		break;
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
	}

	// T (Vertical)
	switch (mesh.Data->VerticalWrap)
	{
	case enums::repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case enums::mirror_repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case enums::clamp_border:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;
	case enums::clamp_edge:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	}
}


Shader Drawer::regularShader_;
Shader Drawer::billboardShader_;

CoordinateGizmo Drawer::coordGizmo_;

int32_t Drawer::uUseVertexColor_ = -1;
int32_t Drawer::uNormalMatrix_ = -1;
int32_t Drawer::uModel_ = -1;
int32_t Drawer::uView_ = -1;
int32_t Drawer::uProjection_ = -1;
int32_t Drawer::uBaseColorTex_ = -1;
int32_t Drawer::uNormalTex_ = -1;
int32_t Drawer::uMetallicRoughtnessTex_ = -1;
int32_t Drawer::uOcclusionTex_ = -1;
int32_t Drawer::uEmissiveTex_ = -1;

int32_t Drawer::uBboardCenterWorld_ = -1;
int32_t Drawer::uBboardScale_ = -1;
int32_t Drawer::uBboardView_ = -1;
int32_t Drawer::uBboardProjection_ = -1;