#pragma once
#include <cstdint>
#include <queue>

#include "BillboardRenderQuery.h"
#include "Shader.h"
#include "Texture.h"

struct RenderNode
{
	BillboardRenderQuery RenderQuery;
	Texture NodeTexture;
};

class CoordinateGizmo
{
private:
	uint32_t vaoId_ = 0;
	uint16_t camId_ = 0;

	RenderNode xNode_;
	RenderNode yNode_;
	RenderNode zNode_;

	std::queue<RenderNode> nodeRenderQuery_;

	// Uniform IDs
	int32_t uAmbientColor_ = -1;
	int32_t uAmbientIntensity_ = -1;
	int32_t uDirLightCount_ = -1;
	int32_t uPointLightCount_ = -1;
	int32_t uSpotlightCount_ = -1;
	int32_t uUseVertexColor_ = -1;
	int32_t uModel_ = -1;
	int32_t uProjection_ = -1;
	int32_t uView_ = -1;
	int32_t uBboardCenterWorld_ = -1;
	int32_t uBboardScale_ = -1;
	int32_t uBboardView_ = -1;
	int32_t uBboardProjection_ = -1;


	void InitUniformIds();
	void BuildVao();
	void BuildCamera();
	void BuildNodeObjects();
	void BuildTextures();

	void UpdateGizmoCam() const;
	void DrawAxes() const;
	void DrawNodes();

	void EmplaceNodes(const Camera* camera);

public:
	CoordinateGizmo() = default;
	void InitGizmo();
	void Draw();
};
