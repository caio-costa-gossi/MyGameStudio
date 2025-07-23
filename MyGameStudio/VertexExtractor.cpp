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
	err = ExtractAllVertices(model, vertices.get(), vertexCount);
	if (err.Code())
		return err;

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

	// Go through children
	for (const uint32_t childId : node.children)
	{
		Err err = CountVerticesNode(model, model.nodes[childId], vertexCount);
		if (err.Code())
			ConsoleManager::PrintWarning("Error processing node vertex count: " + err.Message());
	}

	return error_const::SUCCESS;
}

Err VertexExtractor::ExtractAllVertices(const tinygltf::Model& model, Vertex* vertexList, const uint64_t vertexCount)
{
	const tinygltf::Scene& scene = model.scenes[model.defaultScene];

	for (const uint32_t nodeId : scene.nodes)
	{
		Err err = ExtractVerticesNode(model, model.nodes[nodeId], vertexList, vertexCount);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err VertexExtractor::ExtractVerticesNode(const tinygltf::Model& model, const tinygltf::Node& node, Vertex* vertexList, const uint64_t vertexCount)
{
	// Stack transform matrix
	Err err = StackNodeTransform(node);
	if (err.Code())
		return err;

	// Construct final transform for node
	Transform transform;
	std::stack<Transform> temp = transforms_;
	while (!temp.empty())
	{
		transform = transform * temp.top();
		temp.pop();
	}

	err = CopyVerticesBuffer(model, model.meshes[node.mesh], transform, vertexList, vertexCount);
	if (err.Code())
		return err;

	// Go through children
	for (const uint32_t childId : node.children)
	{
		err = ExtractVerticesNode(model, model.nodes[childId], vertexList, vertexCount);
		if (err.Code())
			ConsoleManager::PrintWarning("Error processing node vertex count: " + err.Message());
	}

	return error_const::SUCCESS;
}

Err VertexExtractor::StackNodeTransform(const tinygltf::Node& node)
{
	if (!node.matrix.empty() && node.matrix.size() != 16)
		return error_const::IMPORT_INVALID_TRANSFORM;

	if (node.matrix.empty())
		return error_const::SUCCESS;

	float matrixVal[16] {};
	for (uint8_t i = 0; i < 16; ++i)
	{
		matrixVal[i] = static_cast<float>(node.matrix[i]);
	}

	auto transform = Transform(matrixVal);
	transforms_.emplace(transform);

	return error_const::SUCCESS;
}

Err VertexExtractor::CopyVerticesBuffer(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const Transform& transform, Vertex* vertexList, const uint64_t vertexCount)
{
	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		if (primitive.attributes.find("POSITION") == primitive.attributes.end())
		{
			ConsoleManager::PrintWarning("POSITION attribute not found. Skipping primitive...");
			continue;
		}

		const tinygltf::Accessor vertexAccessor = model.accessors[primitive.attributes.at("POSITION")];
		if (vertexAccessor.type != TINYGLTF_TYPE_VEC3 || vertexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
		{
			ConsoleManager::PrintWarning("Component type is not float or data type is not vec3. Skipping primitive...");
			continue;
		}

		const uint32_t verticesInPrimitive = static_cast<uint32_t>(vertexAccessor.count);
		if (verticesInPrimitive + vCounter_ > vertexCount)
		{
			ConsoleManager::PrintWarning("VertexCount from primitive will exceed allocated space. Skipping...");
			continue;
		}

		const tinygltf::BufferView vertexBufferView = model.bufferViews[vertexAccessor.bufferView];
		const tinygltf::Buffer vertexBuffer = model.buffers[vertexBufferView.buffer];

		const size_t offset = vertexAccessor.byteOffset + vertexBufferView.byteOffset;
		const size_t stride = vertexBufferView.byteStride ? vertexBufferView.byteStride : 3 * sizeof(float);

		for (uint32_t i = 0; i < vertexAccessor.count; ++i)
		{
			const float* vertexData = reinterpret_cast<const float*>(vertexBuffer.data.data() + offset + stride * i);

			Vertex newVertex;

			newVertex.Color = { 255,255,255,255 };
			newVertex.Pos.X = vertexData[0];
			newVertex.Pos.Y = vertexData[1];
			newVertex.Pos.Z = vertexData[2];

			// APPLY TRANSFORM

			vertexList[vCounter_ + i] = newVertex;
		}

		vCounter_ += verticesInPrimitive;
	}

	return error_const::SUCCESS;
}


std::stack<Transform> VertexExtractor::transforms_ = std::stack<Transform>();
uint64_t VertexExtractor::vCounter_ = 0;