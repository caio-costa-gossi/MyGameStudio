#include "VertexExtractor.h"

#include <tiny_gltf.h>

#include "ConsoleManager.h"

Err VertexExtractor::ExtractVertices(const tinygltf::Model& model, std::unique_ptr<Vertex[]>& vertices, uint64_t& vertexCount)
{
	// Get vertex count
	uint64_t totalVertexCount = 0;
	Err err = CountVertices(model, totalVertexCount);
	if (err.Code())
		return err;

	// Prepare vertex buffer
	vertices = std::make_unique<Vertex[]>(totalVertexCount);

	// Start extraction
	err = GetAllVertices(model, vertices, vertexCount);

	return error_const::SUCCESS;
}

Err VertexExtractor::CountVertices(const tinygltf::Model& model, uint64_t& vertexCount)
{
	const tinygltf::Scene& scene = model.scenes[model.defaultScene];

	for (const uint32_t nodeId : scene.nodes)
	{
		Err err = CountVerticesNode(model, model.nodes[nodeId], vertexCount);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err VertexExtractor::CountVerticesNode(const tinygltf::Model& model, const tinygltf::Node& node, uint64_t& vertexCount)
{
	// Calculate node vertex count
	if (node.mesh < 0)
	{
		ConsoleManager::PrintWarning("Node mesh index < 0");
		return error_const::SUCCESS;
	}

	const tinygltf::Mesh& mesh = model.meshes[node.mesh];

	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		if (primitive.attributes.find("POSITION") == primitive.attributes.end())
		{
			ConsoleManager::PrintWarning("Primitive does not contain POSITION data. Skipping...");
			continue;
		}

		const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("POSITION")];
		vertexCount += accessor.count;
	}

	// Go through the other nodes
	for (const uint32_t childId : node.children)
	{
		Err err = CountVerticesNode(model, model.nodes[childId], vertexCount);
		if (err.Code())
			ConsoleManager::PrintWarning("Error processing node vertex count: " + err.Message());
	}

	return error_const::SUCCESS;
}



//std::stack<glm::mat4> VertexExtractor::transforms_;
uint64_t VertexExtractor::vCounter_ = 0;