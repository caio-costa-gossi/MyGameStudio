#pragma once
#include <queue>

#include "RenderQuery.h"
#include "RenderRequest.h"
#include "Shader.h"

class ModelDrawer
{
private:
	static Shader modelShader_;
	static std::queue<RenderQuery> modelRenderQueue_;

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

	// Private functions
	static Err InitShader();
	static Err InitUniformIds();

	static void SetShaderConfig();
	static void SetTextureWrapping(const RenderQuery& query);
	static void SetStaticUniforms();
	static void SetShaderUniforms(const RenderQuery& query);

	static Err AddMeshTextures(const Mesh& mesh);
	static Err DrawQuery(const RenderQuery& query);

public:
	static Err Init();
	static Err RequestDraw(const RenderRequest& request);
	static Err Draw();

	static Shader& GetShader();
};
