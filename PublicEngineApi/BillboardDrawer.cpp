#include "BillboardDrawer.h"

#include <glad/glad.h>

#include "DrawingCache.h"
#include "GameConsoleManager.h"

Err BillboardDrawer::Init()
{
	Err err = InitShader();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err BillboardDrawer::RequestDraw(const BillboardRenderRequest& request)
{
	// Create OpenGL Vertex Attribute Object and upload vertex data saving it in a list / use VAO list previously created
	BillboardRenderQuery newQuery = { request.Data, DrawingCache::GetBillboardVao(request.Data.BillboardImageId) };
	billboardRenderQueue_.emplace(newQuery);

	// Preload billboard texture
	Err err = DrawingCache::StoreTexture(request.Data.BillboardImageId);
	if (err.Code())
		GameConsoleManager::PrintError(err);

	return error_const::SUCCESS;
}

Err BillboardDrawer::Draw()
{
	billboardShader_.Use();
	while (!billboardRenderQueue_.empty())
	{
		const BillboardRenderQuery& query = billboardRenderQueue_.top();
		DrawQuery(query);
		billboardRenderQueue_.pop();
	}

	return error_const::SUCCESS;
}

void BillboardDrawer::DrawQuery(const BillboardRenderQuery& query)
{
	// Prepare to draw
	SetShaderConfig();
	SetShaderUniforms(query);
	DrawingCache::GetTexture(query.BillboardData.BillboardImageId).Use(0);

	// Draw
	glBindVertexArray(query.BillboardVao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void BillboardDrawer::SetShaderConfig()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void BillboardDrawer::SetShaderUniforms(const BillboardRenderQuery& query)
{
	// Transforms
	Shader::SetUniform(uCenterWorld_, query.BillboardData.WorldPos.X, query.BillboardData.WorldPos.Y, query.BillboardData.WorldPos.Z);
	Shader::SetUniform(uScale_, query.BillboardData.Scale.X, query.BillboardData.Scale.Y);
	Shader::SetUniform(uView_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetView().GetData(), false);
	Shader::SetUniform(uProjection_, enums::MatrixDim::m4x4, CameraManager::GetMainCamera()->GetProjection().GetData(), false);
}

Err BillboardDrawer::InitShader()
{
	GameConsoleManager::PrintInfo("Initializing billboard shader...", enums::ConsoleMessageSender::render);

	// Billboard shader
	Err err = billboardShader_.Init("shaders/BillboardShader.vert", "shaders/BillboardShader.frag");
	if (err.Code())
		return err;

	err = billboardShader_.Build();
	if (err.Code())
		return err;

	err = InitUniformIds();
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err BillboardDrawer::InitUniformIds()
{
	billboardShader_.Use();

	uCenterWorld_ = billboardShader_.GetUniformId("centerWorld");
	uScale_ = billboardShader_.GetUniformId("scale");
	uView_ = billboardShader_.GetUniformId("view");
	uProjection_ = billboardShader_.GetUniformId("projection");

	return error_const::SUCCESS;
}

Shader& BillboardDrawer::GetShader()
{
	return billboardShader_;
}


Shader BillboardDrawer::billboardShader_;
std::priority_queue<BillboardRenderQuery> BillboardDrawer::billboardRenderQueue_;

int32_t BillboardDrawer::uCenterWorld_ = -1;
int32_t BillboardDrawer::uScale_ = -1;
int32_t BillboardDrawer::uView_ = -1;
int32_t BillboardDrawer::uProjection_ = -1;