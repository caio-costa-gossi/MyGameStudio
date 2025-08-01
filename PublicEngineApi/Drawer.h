#pragma once
#include <queue>
#include <unordered_map>

#include "BillboardRenderQuery.h"
#include "CameraInstance.h"
#include "CoordinateGizmo.h"
#include "MeshInstance.h"
#include "RenderQuery.h"
#include "Shader.h"
#include "Texture.h"

using TextureList = std::unordered_map<uint32_t, Texture>;

class Drawer
{
private:
	static Shader regularShader_;
	static Shader billboardShader_;

	static CoordinateGizmo coordGizmo_;

	// Uniform IDs
	static int32_t uUseVertexColor_;
	static int32_t uNormalMatrix_;
	static int32_t uModel_;
	static int32_t uView_;
	static int32_t uProjection_;
	static int32_t uBaseColorTex_;
	static int32_t uNormalTex_;
	static int32_t uMetallicRoughtnessTex_;
	static int32_t uOcclusionTex_;
	static int32_t uEmissiveTex_;

	static int32_t uBboardCenterWorld_;
	static int32_t uBboardScale_;
	static int32_t uBboardView_;
	static int32_t uBboardProjection_;
	

	static Err InitShaders();
	static Err InitUniformIdsRegular();
	static Err InitUniformIdsBillboard();

	static void DrawRegular(const RenderQuery& query, const TextureList& textures);
	static void DrawBillboard(const BillboardRenderQuery& query, const TextureList& textures);

	static void SetShaderConfig();
	static void SetTextureWrapping(const MeshInstance& mesh);

	static void SetStaticUniformsRegular();
	static void SetShaderUniformsRegular(const RenderQuery& query);
	static void SetShaderUniformsBillboard(Shader& shader, const BillboardRenderQuery& query);

public:
	static Err Init();
	static void Draw(std::queue<RenderQuery>& queries, std::priority_queue<BillboardRenderQuery>& billboardQueries, const TextureList& textures);
};