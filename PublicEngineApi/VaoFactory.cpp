#include "VaoFactory.h"

#include <glad/glad.h>

Err VaoFactory::NewAttribObject(const Mesh& mesh, uint32_t& newVaoId)
{
	// Setup Vertex Array Object
	uint32_t newVao;
	glGenVertexArrays(1, &newVao);
	glBindVertexArray(newVao);
	newVaoId = newVao;

	// Setup Vertex Buffer Object
	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(mesh.VertexCount * sizeof(Vertex)), mesh.VertexList.get(), GL_STATIC_DRAW);

	// Setup Element Buffer Object
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(mesh.IndexCount * sizeof(uint32_t)), mesh.IndexList.get(), GL_STATIC_DRAW);

	// Define vertex attribute layout
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	// Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Color
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// BaseColorTexCoord
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// NormalTexCoord
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);

	// MetallicRoughnessTexCoord
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(14 * sizeof(float)));
	glEnableVertexAttribArray(5);

	// OcclusionTexCoord
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(16 * sizeof(float)));
	glEnableVertexAttribArray(6);

	// EmissiveTexCoord
	glVertexAttribPointer(7, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(18 * sizeof(float)));
	glEnableVertexAttribArray(7);


	// Unbind VAO & VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return error_const::SUCCESS;
}

Err VaoFactory::NewBillboardAttribObject(uint32_t& newVaoId)
{
	// Setup Vertex Array Object
	uint32_t newVao;
	glGenVertexArrays(1, &newVao);
	glBindVertexArray(newVao);
	newVaoId = newVao;

	// Prepare billboard vertex data
	const float quadVertices[] = {
		-0.5f, -0.5f,  // bottom-left
		 0.5f, -0.5f,  // bottom-right
		 0.5f,  0.5f,  // top-right
		-0.5f,  0.5f   // top-left
	};

	const uint32_t quadIndices[] = {
		0, 1, 2, 
		2, 3, 0 
	};

	// Setup Vertex Buffer Object
	uint32_t vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int32_t>(4 * 2 * sizeof(float)), quadVertices, GL_STATIC_DRAW);

	// Setup Element Buffer Object
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int32_t>(6 * sizeof(uint32_t)), quadIndices, GL_STATIC_DRAW);

	// Define vertex attribute layout
	// QuadPos
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// Unbind VAO & VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return error_const::SUCCESS;
}