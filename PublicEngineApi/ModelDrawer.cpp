#include "ModelDrawer.h"

#include <glad/glad.h>

#include "CameraManager.h"
#include "DrawingCache.h"
#include "GameConsoleManager.h"
#include "NumericUtils.h"

Err ModelDrawer::Init()
{
	Err err = InitShader();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err ModelDrawer::RequestDraw(const RenderRequest& request)
{
	const std::vector<uint32_t>& modelVaoList = DrawingCache::GetModelVaoList(*request.Model);

	// Create OpenGL Vertex Attribute Object and upload vertex data saving it in a list / use VAO list previously created
	for (uint32_t i = 0; i < request.Model->MeshCount; ++i)
	{
		RenderQuery newQuery = { &request.Model->Meshes[i], modelVaoList[i], request.Transform };
		modelRenderQueue_.emplace(newQuery);
	}

	// Preload textures from meshes
	for (uint32_t i = 0; i < request.Model->MeshCount; ++i)
	{
		Err err = AddMeshTextures(request.Model->Meshes[i]);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err ModelDrawer::Draw()
{
	// Draw 3d meshes with standard shader
	modelShader_.Use();
	SetShaderConfig();
	SetStaticUniforms();

	while (!modelRenderQueue_.empty())
	{
		const RenderQuery& query = modelRenderQueue_.front();
		DrawQuery(query);
		modelRenderQueue_.pop();
	}

	return error_const::SUCCESS;
}

void ModelDrawer::SetShaderConfig()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void ModelDrawer::SetStaticUniforms()
{
	// Material textures
	Shader::SetUniform(uBaseColorTex_, enums::TextureMap::base_color);
	Shader::SetUniform(uNormalTex_, enums::TextureMap::normal);
	Shader::SetUniform(uMetallicRoughtnessTex_, enums::TextureMap::metallic_roughness);
	Shader::SetUniform(uOcclusionTex_, enums::TextureMap::occlusion);
	Shader::SetUniform(uEmissiveTex_, enums::TextureMap::emissive);
}

Err ModelDrawer::InitShader()
{
	GameConsoleManager::PrintInfo("Initializing model shader...", enums::ConsoleMessageSender::render);

	// Standard shader
	Err err = modelShader_.Init("shaders/RegularShader.vert", "shaders/RegularShader.frag");
	if (err.Code())
		return err;

	err = modelShader_.Build();
	if (err.Code())
		return err;

	err = InitUniformIds();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err ModelDrawer::InitUniformIds()
{
	modelShader_.Use();

	// Aux
	uUseVertexColor_ = modelShader_.GetUniformId("useVertexColor");
	uNormalMatrix_ = modelShader_.GetUniformId("normalMatrix");

	// Transforms
	uModel_ = modelShader_.GetUniformId("model");
	uView_ = modelShader_.GetUniformId("view");
	uProjection_ = modelShader_.GetUniformId("projection");

	// Material textures
	uBaseColorTex_ = modelShader_.GetUniformId("materialProps.baseColorTex");
	uNormalTex_ = modelShader_.GetUniformId("materialProps.normalTex");
	uMetallicRoughtnessTex_ = modelShader_.GetUniformId("materialProps.metallicRoughnessTex");
	uOcclusionTex_ = modelShader_.GetUniformId("materialProps.occlusionTex");
	uEmissiveTex_ = modelShader_.GetUniformId("materialProps.emissiveTex");

	return error_const::SUCCESS;
}


Err ModelDrawer::DrawQuery(const RenderQuery& query)
{
	// Prepare to draw
	SetShaderUniforms(query);
	SetTextureWrapping(query);

	// Load textures into their OpenGL units
	if (query.Data->Material.BaseColorTexture >= 0)
		DrawingCache::GetTexture(query.Data->Material.BaseColorTexture).Use(enums::TextureMap::base_color);

	if (query.Data->Material.NormalTexture >= 0)
		DrawingCache::GetTexture(query.Data->Material.NormalTexture).Use(enums::TextureMap::normal);

	if (query.Data->Material.MetallicRoughnessTexture >= 0)
		DrawingCache::GetTexture(query.Data->Material.MetallicRoughnessTexture).Use(enums::TextureMap::metallic_roughness);

	if (query.Data->Material.OcclusionTexture >= 0)
		DrawingCache::GetTexture(query.Data->Material.OcclusionTexture).Use(enums::TextureMap::occlusion);

	if (query.Data->Material.EmissiveTexture >= 0)
		DrawingCache::GetTexture(query.Data->Material.EmissiveTexture).Use(enums::TextureMap::emissive);

	// Draw
	glBindVertexArray(query.ArrayObjectId);
	glDrawElements(GL_TRIANGLES, static_cast<int32_t>(query.Data->IndexCount), GL_UNSIGNED_INT, nullptr);

	return error_const::SUCCESS;
}

void ModelDrawer::SetShaderUniforms(const RenderQuery& query)
{
	Shader::SetUniform(uUseVertexColor_, query.Data->Material.BaseColorTexture < 0);

	// Pre-calculate normal matrix for model
	Shader::SetUniform(uNormalMatrix_, enums::MatrixDim::m3x3, NumericUtils::CalculateNormalMatrix(query.Model).GetData(), false);

	// Transforms
	Shader::SetUniform(uModel_, enums::MatrixDim::m4x4, query.Model.GetData(), false);
	Shader::SetUniform(uView_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetView().GetData(), false);
	Shader::SetUniform(uProjection_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetProjection().GetData(), false);
}

Err ModelDrawer::AddMeshTextures(const Mesh& mesh)
{
	if (mesh.Material.BaseColorTexture >= 0)
	{
		Err err = DrawingCache::StoreTexture(mesh.Material.BaseColorTexture);
		if (err.Code())
			GameConsoleManager::PrintError(err);
	}

	if (mesh.Material.NormalTexture >= 0)
	{
		Err err = DrawingCache::StoreTexture(mesh.Material.NormalTexture);
		if (err.Code())
			GameConsoleManager::PrintError(err);
	}

	if (mesh.Material.MetallicRoughnessTexture >= 0)
	{
		Err err = DrawingCache::StoreTexture(mesh.Material.MetallicRoughnessTexture);
		if (err.Code())
			GameConsoleManager::PrintError(err);
	}

	if (mesh.Material.OcclusionTexture >= 0)
	{
		Err err = DrawingCache::StoreTexture(mesh.Material.OcclusionTexture);
		if (err.Code())
			GameConsoleManager::PrintError(err);
	}

	if (mesh.Material.EmissiveTexture >= 0)
	{
		Err err = DrawingCache::StoreTexture(mesh.Material.EmissiveTexture);
		if (err.Code())
			GameConsoleManager::PrintError(err);
	}

	return error_const::SUCCESS;
}

void ModelDrawer::SetTextureWrapping(const RenderQuery& query)
{
	// S (Horizontal)
	switch (query.Data->HorizontalWrap)
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
	switch (query.Data->VerticalWrap)
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

Shader& ModelDrawer::GetShader()
{
	return modelShader_;
}


Shader ModelDrawer::modelShader_;
std::queue<RenderQuery> ModelDrawer::modelRenderQueue_;

int32_t ModelDrawer::uUseVertexColor_ = -1;
int32_t ModelDrawer::uNormalMatrix_ = -1;
int32_t ModelDrawer::uModel_ = -1;
int32_t ModelDrawer::uView_ = -1;
int32_t ModelDrawer::uProjection_ = -1;
int32_t ModelDrawer::uBaseColorTex_ = -1;
int32_t ModelDrawer::uNormalTex_ = -1;
int32_t ModelDrawer::uMetallicRoughtnessTex_ = -1;
int32_t ModelDrawer::uOcclusionTex_ = -1;
int32_t ModelDrawer::uEmissiveTex_ = -1;