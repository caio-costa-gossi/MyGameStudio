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

	void BuildVao();
	void BuildCamera();
	void BuildNodeObjects();
	void BuildTextures();

	void UpdateGizmoCam() const;
	void DrawAxes(const Shader& regularShader) const;
	void DrawNodes(const Shader& billboardShader);

	void EmplaceNodes(const Camera* camera);

public:
	CoordinateGizmo() = default;
	void InitGizmo();
	void Draw(const Shader& regularShader, const Shader& billboardShader);
};
