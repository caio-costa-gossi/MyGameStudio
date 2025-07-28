#include "Drawer.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "CameraManager.h"
#include "GameConsoleManager.h"
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

void Drawer::Draw(std::queue<RenderQuery>& queries, const TextureList& textures)
{
	// Reset color
	glClearColor(1.0f, 1.0f, 0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	while (!queries.empty())
	{
		// Decide shader & configure
		const RenderQuery& query = queries.front();
		const Shader& shader = query.IsBillboard ? billboardShader_ : regularShader_;

		shader.Use();
		SetShaderConfig();

		SetShaderUniforms(shader, query);
		SetTextureWrapping(query.MeshInstance);
		textures.at(query.MeshInstance.Data->TextureAssetId).Use();

		// Draw
		glBindVertexArray(query.MeshInstance.ArrayObjectId);
		glDrawElements(GL_TRIANGLES, static_cast<int32_t>(query.MeshInstance.Data->IndexCount), GL_UNSIGNED_INT, nullptr);

		queries.pop();
	}

	coordGizmo_.Draw(regularShader_);
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

void Drawer::SetShaderConfig()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Drawer::SetShaderUniforms(const Shader& shader, const RenderQuery& query)
{
	shader.SetUniform("useVertexColor", query.MeshInstance.Data->UseVertexColor);
	shader.SetUniform("light", 1.0f, 1.0f, 1.0f);

	// Transforms
	shader.SetUniform("model", enums::MatrixDim::m4x4, query.Model.GetData(), false);
	shader.SetUniform("view", enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetView().GetData(), false);
	shader.SetUniform("projection", enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetProjection().GetData(), false);
}

Shader Drawer::regularShader_;
Shader Drawer::billboardShader_;

CoordinateGizmo Drawer::coordGizmo_;