#pragma once
#include <queue>
#include <unordered_map>

#include "MeshInstance.h"
#include "RenderQuery.h"
#include "Shader.h"
#include "Texture.h"

using TextureList = std::unordered_map<uint32_t, Texture>;

class Drawer
{
private:
	static void SetTextureWrapping(const MeshInstance& mesh);
	static void SetShaderConfig();

public:
	static void Draw(const Shader& shader, std::queue<RenderQuery>& queries, const TextureList& textures);
};