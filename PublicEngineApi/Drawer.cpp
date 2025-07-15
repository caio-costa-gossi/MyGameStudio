#include "Drawer.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>

void Drawer::Draw(const Shader& shader, const MeshList& meshes, const TextureList& textures)
{
	glClearColor(1.0f, 1.0f, 0, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader.Use();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (const auto& pair : meshes)
	{
		SetTextureWrapping(pair.second);

		textures.at(pair.second.Data.TextureAssetId).Use();

		glBindVertexArray(pair.second.ArrayObjectId);
		glDrawElements(GL_TRIANGLES, static_cast<int32_t>(pair.second.Data.IndexCount), GL_UNSIGNED_INT, nullptr);
	}
}

void Drawer::SetTextureWrapping(const MeshInstance& mesh)
{
	// S (Horizontal)
	switch (mesh.Data.HorizontalWrap)
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
	switch (mesh.Data.VerticalWrap)
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
