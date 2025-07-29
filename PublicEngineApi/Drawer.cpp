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

	coordGizmo_.InitGizmo();

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

	// Billboard shader
	err = billboardShader_.Init("shaders/BillboardShader.vert", "shaders/BillboardShader.frag");
	if (err.Code())
		return err;

	err = billboardShader_.Build();
	if (err.Code())
		return err;

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

	// Draw 3d meshes
	while (!queries.empty())
	{
		// Use standard shader
		const RenderQuery& query = queries.front();
		regularShader_.Use();
		DrawRegular(query, textures);

		queries.pop();
	}

	// Draw billboards
	while (!billboardQueries.empty())
	{
		const BillboardRenderQuery& query = billboardQueries.top();
		billboardShader_.Use();
		DrawBillboard(query, textures);

		billboardQueries.pop();
	}

	coordGizmo_.Draw(regularShader_, billboardShader_);
}

void Drawer::DrawRegular(const RenderQuery& query, const TextureList& textures)
{
	// Prepare to draw
	SetShaderConfig();
	SetShaderUniformsRegular(regularShader_, query);
	SetTextureWrapping(query.MeshInstance);
	textures.at(query.MeshInstance.Data->TextureAssetId).Use();

	// Draw
	glBindVertexArray(query.MeshInstance.ArrayObjectId);
	glDrawElements(GL_TRIANGLES, static_cast<int32_t>(query.MeshInstance.Data->IndexCount), GL_UNSIGNED_INT, nullptr);
}

void Drawer::DrawBillboard(const BillboardRenderQuery& query, const TextureList& textures)
{
	// Prepare to draw
	SetShaderConfig();
	SetShaderUniformsBillboard(billboardShader_, query);
	textures.at(query.BillboardData.BillboardImageId).Use();

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

void Drawer::SetShaderUniformsRegular(const Shader& shader, const RenderQuery& query)
{
	shader.SetUniform("useVertexColor", query.MeshInstance.Data->UseVertexColor);

	// Pre-calculate normal matrix for model
	shader.SetUniform("normalMatrix", enums::MatrixDim::m3x3, NumericUtils::CalculateNormalMatrix(query.Model).GetData(), false);

	// Transforms
	shader.SetUniform("model", enums::MatrixDim::m4x4, query.Model.GetData(), false);
	shader.SetUniform("view", enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetView().GetData(), false);
	shader.SetUniform("projection", enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetProjection().GetData(), false);
}

void Drawer::SetShaderUniformsBillboard(const Shader& shader, const BillboardRenderQuery& query)
{
	// Transforms
	shader.SetUniform("centerWorld", query.BillboardData.WorldPos.X, query.BillboardData.WorldPos.Y, query.BillboardData.WorldPos.Z);
	shader.SetUniform("scale", query.BillboardData.Scale.X, query.BillboardData.Scale.Y);
	shader.SetUniform("view", enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetView().GetData(), false);
	shader.SetUniform("projection", enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetProjection().GetData(), false);
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