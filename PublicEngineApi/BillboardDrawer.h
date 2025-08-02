#pragma once
#include <queue>

#include "BillboardRenderQuery.h"
#include "BillboardRenderRequest.h"
#include "Shader.h"

class BillboardDrawer
{
private:
	static Shader billboardShader_;
	static std::priority_queue<BillboardRenderQuery> billboardRenderQueue_;

	static int32_t uCenterWorld_;
	static int32_t uScale_;
	static int32_t uView_;
	static int32_t uProjection_;

	static Err InitShader();
	static Err InitUniformIds();

	static void SetShaderConfig();
	static void SetShaderUniforms(const BillboardRenderQuery& query);
	static void DrawQuery(const BillboardRenderQuery& query);

public:
	static Err Init();
	static Err RequestDraw(const BillboardRenderRequest& request);
	static Err Draw();

	static Shader& GetShader();
};
