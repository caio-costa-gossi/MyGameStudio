#include "Drawer.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "Transform.h"

void Drawer::Draw(const Shader& shader, std::queue<RenderQuery>& queries, const TextureList& textures)
{
	glClearColor(1.0f, 1.0f, 0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.Use();
	SetShaderConfig();

	while (!queries.empty())
	{
		const RenderQuery& query = queries.front();

		SetShaderUniforms(shader, query);
		SetTextureWrapping(query.MeshInstance);
		textures.at(query.MeshInstance.Data->TextureAssetId).Use();

		glBindVertexArray(query.MeshInstance.ArrayObjectId);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, static_cast<int32_t>(query.MeshInstance.Data->IndexCount), GL_UNSIGNED_INT, nullptr);

		queries.pop();
	}
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
	shader.SetUniform("model", enums::MatrixDim::m4x4, query.Model.GetData(), false);
	shader.SetUniform("view", enums::MatrixDim::m4x4, query.View.GetData(), false);
	shader.SetUniform("projection", enums::MatrixDim::m4x4, query.Projection.GetData(), false);
}
