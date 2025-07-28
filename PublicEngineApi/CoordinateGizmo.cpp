#include "CoordinateGizmo.h"

#include <glad/glad.h>

#include "BillboardRenderRequest.h"
#include "Camera.h"
#include "CameraManager.h"
#include "Shader.h"
#include "Transform.h"
#include "VaoFactory.h"
#include "Vertex.h"
#include "WindowManager.h"

void CoordinateGizmo::InitGizmo()
{
	BuildVao();
	BuildBillboardVaos();
	BuildCamera();
	BuildTextures();
}

void CoordinateGizmo::BuildVao()
{
	// Gizmo Attribute Object
	glGenVertexArrays(1, &vaoId_);
	glBindVertexArray(vaoId_);

	// Setup Vertex Buffer Object
	Vertex gizmoVertices[6] = {
		{ {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
		{ {0.9f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.9f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
		{ {0.0f, 0.0f, 0.9f}, {0.0f, 0.0f, 1.0f, 1.0f}}
	};

	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(6 * sizeof(Vertex)), gizmoVertices, GL_STATIC_DRAW);

	// Define vertex attribute layout
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TextCoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind VAO & VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CoordinateGizmo::BuildBillboardVaos()
{
	xNodeQuery_ = { { { 0.9f, -0.2f, 0.0f }, { 0.3f, 0.3f }, 0 }, 0 };
	yNodeQuery_ = { { { 0.0f, 0.8f, 0.0f }, { 0.3f, 0.3f }, 0 }, 0 };
	zNodeQuery_ = { { { 0.0f, -0.2f, 0.9f }, { 0.3f, 0.3f }, 0 }, 0 };

	VaoFactory::NewBillboardAttribObject(xNodeQuery_.BillboardVao);
	VaoFactory::NewBillboardAttribObject(yNodeQuery_.BillboardVao);
	VaoFactory::NewBillboardAttribObject(zNodeQuery_.BillboardVao);
}

void CoordinateGizmo::BuildCamera()
{
	CameraManager::CreatePerspective(camId_, enums::free_cam, 600, 600, 90, 0.01f, 100.0f);
}

void CoordinateGizmo::BuildTextures()
{
	// xNode
	glGenTextures(1, &xNodeQuery_.BillboardData.BillboardImageId);
	glBindTexture(GL_TEXTURE_2D, xNodeQuery_.BillboardData.BillboardImageId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xNodeImage_.Width, xNodeImage_.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, xNodeImage_.Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// yNode
	glGenTextures(1, &yNodeQuery_.BillboardData.BillboardImageId);
	glBindTexture(GL_TEXTURE_2D, yNodeQuery_.BillboardData.BillboardImageId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, yNodeImage_.Width, yNodeImage_.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, yNodeImage_.Data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// zNode
	glGenTextures(1, &zNodeQuery_.BillboardData.BillboardImageId);
	glBindTexture(GL_TEXTURE_2D, zNodeQuery_.BillboardData.BillboardImageId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, zNodeImage_.Width, zNodeImage_.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, zNodeImage_.Data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void CoordinateGizmo::UpdateGizmoCam() const
{
	Camera* gizmoCam = CameraManager::GetCamera(camId_);
	gizmoCam->ChangePitch(-CameraManager::GetMainCamera()->GetPitch());
	gizmoCam->ChangeYaw(-CameraManager::GetMainCamera()->GetYaw());
}

void CoordinateGizmo::Draw(const Shader& regularShader, const Shader& billboardShader)
{
	glViewport(0, 2 * WindowManager::GetWindowHeight() / 3, WindowManager::GetWindowWidth() / 3, WindowManager::GetWindowHeight() / 3);

	UpdateGizmoCam();
	DrawAxes(regularShader);
	DrawNodes(billboardShader);
}

void CoordinateGizmo::DrawAxes(const Shader& regularShader) const
{
	regularShader.Use();

	Transform transform;
	transform.Translate({ 0.0f, -0.1f, 0.0f });

	const Camera* gizmoCam = CameraManager::GetCamera(camId_);

	// Aux uniforms
	regularShader.SetUniform("light", 1.0f, 1.0f, 1.0f);
	regularShader.SetUniform("useVertexColor", true);

	// Transforms
	regularShader.SetUniform("model", enums::MatrixDim::m4x4, transform.GetData(), false);
	regularShader.SetUniform("projection", enums::MatrixDim::m4x4, transform.GetData(), false);
	regularShader.SetUniform("view", enums::MatrixDim::m4x4, gizmoCam->GetView().GetData(), false);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vaoId_);
	glLineWidth(5.0f);

	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	glLineWidth(1.0f);
}

void CoordinateGizmo::DrawNodes(const Shader& billboardShader)
{
	billboardShader.Use();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Emplace to sort the nodes by distance
	const Camera* gizmoCam = CameraManager::GetCamera(camId_);
	EmplaceNodes(gizmoCam);

	while (!nodeRenderQuery_.empty())
	{
		const BillboardRenderQuery& query = nodeRenderQuery_.front();

		// Prepare node billboard uniforms
		billboardShader.SetUniform("centerWorld", query.BillboardData.WorldPos.X, query.BillboardData.WorldPos.Y, query.BillboardData.WorldPos.Z);
		billboardShader.SetUniform("scale", query.BillboardData.Scale.X, query.BillboardData.Scale.Y);
		billboardShader.SetUniform("view", enums::MatrixDim::m4x4, gizmoCam->GetView().GetData(), false);
		billboardShader.SetUniform("projection", enums::MatrixDim::m4x4, Transform().GetData(), false);

		// Prepare node texture
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, query.BillboardData.BillboardImageId);

		// Draw
		glBindVertexArray(query.BillboardVao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		nodeRenderQuery_.pop();
	}
}

void CoordinateGizmo::EmplaceNodes(const Camera* camera)
{
	if (camera->GetYaw() > 90)
	{
		nodeRenderQuery_.emplace(xNodeQuery_);
		nodeRenderQuery_.emplace(yNodeQuery_);
		nodeRenderQuery_.emplace(zNodeQuery_);
	}
	else
	{
		nodeRenderQuery_.emplace(zNodeQuery_);
		nodeRenderQuery_.emplace(yNodeQuery_);
		nodeRenderQuery_.emplace(xNodeQuery_);
	}
}
