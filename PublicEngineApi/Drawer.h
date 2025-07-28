#pragma once
#include <queue>
#include <unordered_map>

#include "CameraInstance.h"
#include "CoordinateGizmo.h"
#include "MeshInstance.h"
#include "RenderQuery.h"
#include "Shader.h"
#include "Texture.h"
#include "Viewport.h"

using TextureList = std::unordered_map<uint32_t, Texture>;

class Drawer
{
private:
	static Shader regularShader_;
	static Shader billboardShader_;

	static CoordinateGizmo coordGizmo_;

	static Err InitShaders();

	static void SetTextureWrapping(const MeshInstance& mesh);
	static void SetShaderConfig();
	static void SetShaderUniforms(const Shader& shader, const RenderQuery& query);

public:
	static Err Init();
	static void Draw(std::queue<RenderQuery>& queries, const TextureList& textures);
};