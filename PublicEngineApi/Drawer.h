#pragma once
#include <unordered_map>

#include "MeshInstance.h"
#include "Shader.h"
#include "Texture.h"

using MeshList = std::unordered_map<uint32_t, MeshInstance>;
using TextureList = std::unordered_map<uint32_t, Texture>;

class Drawer
{
private:
	static void SetTextureWrapping(const MeshInstance& mesh);
	static void SetShaderConfig();

public:
	static void Draw(const Shader& shader, const MeshList& meshes, const TextureList& textures);
};