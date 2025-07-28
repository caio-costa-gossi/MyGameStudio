#pragma once
#include <cstdint>
#include <queue>

#include "BillboardRenderQuery.h"
#include "Image.h"
#include "Shader.h"

class CoordinateGizmo
{
private:
	uint32_t vaoId_ = 0;
	uint16_t camId_ = 0;

	BillboardRenderQuery xNodeQuery_;
	BillboardRenderQuery yNodeQuery_;
	BillboardRenderQuery zNodeQuery_;

	Image xNodeImage_ = Image("internal_engine_assets/X.png");
	Image yNodeImage_ = Image("internal_engine_assets/Y.png");
	Image zNodeImage_ = Image("internal_engine_assets/Z.png");

	std::queue<BillboardRenderQuery> nodeRenderQuery_;

	void BuildVao();
	void BuildCamera();
	void BuildBillboardVaos();
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
