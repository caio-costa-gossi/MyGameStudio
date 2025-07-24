#include "VertexIndexExtractor.h"

#include <tiny_gltf.h>

#include "ConsoleManager.h"

VertexIndexExtractor::VertexIndexExtractor(tinygltf::Model model) :
	model_(std::move(model)) { }

Err VertexIndexExtractor::ExtractVertices(std::unique_ptr<Vertex[]>& vertices, uint64_t& vertexCount, std::unique_ptr<uint32_t>& indices, uint64_t& indexCount)
{
	// Get vertex count
	Err err = CountVertices();
	if (err.Code())
		return err;

	// Prepare vertex buffer
	vertices = std::make_unique<Vertex[]>(totalVertexCount_);
	vertexList_ = vertices.get();

	// Start extraction
	err = ExtractAllVertices();
	if (err.Code())
		return err;

	vertexCount = totalVertexCount_;

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::CountVertices()
{
	const tinygltf::Scene& scene = model_.scenes[model_.defaultScene];

	for (const uint32_t nodeId : scene.nodes)
	{
		Err err = CountVerticesNode(model_.nodes[nodeId]);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::CountVerticesNode(const tinygltf::Node& node)
{
	// Calculate node vertex count
	if (node.mesh < 0)
	{
		ConsoleManager::PrintWarning("Node mesh index < 0");
		return error_const::SUCCESS;
	}

	const tinygltf::Mesh& mesh = model_.meshes[node.mesh];

	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		if (primitive.attributes.find("POSITION") == primitive.attributes.end())
		{
			ConsoleManager::PrintWarning("Primitive does not contain POSITION data. Skipping...");
			continue;
		}

		const tinygltf::Accessor& accessor = model_.accessors[primitive.attributes.at("POSITION")];
		totalVertexCount_ += accessor.count;
	}

	// Go through children
	for (const uint32_t childId : node.children)
	{
		Err err = CountVerticesNode(model_.nodes[childId]);
		if (err.Code())
			ConsoleManager::PrintWarning("Error processing node vertex count: " + err.Message());
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractAllVertices()
{
	const tinygltf::Scene& scene = model_.scenes[model_.defaultScene];

	for (const uint32_t nodeId : scene.nodes)
	{
		Err err = ExtractVerticesNode(model_.nodes[nodeId]);
		if (err.Code())
			return err;
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::ExtractVerticesNode(const tinygltf::Node& node)
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

	err = CopyVerticesBuffer(model_.meshes[node.mesh], transform);
	if (err.Code())
		return err;

	// Go through children
	for (const uint32_t childId : node.children)
	{
		err = ExtractVerticesNode(model_.nodes[childId]);
		if (err.Code())
			ConsoleManager::PrintWarning("Error processing node vertex count: " + err.Message());
	}

	return error_const::SUCCESS;
}

Err VertexIndexExtractor::StackNodeTransform(const tinygltf::Node& node)
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

Err VertexIndexExtractor::CopyVerticesBuffer(const tinygltf::Mesh& mesh, const Transform& transform)
{
	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		if (primitive.attributes.find("POSITION") == primitive.attributes.end())
		{
			ConsoleManager::PrintWarning("POSITION attribute not found. Skipping primitive...");
			continue;
		}

		const tinygltf::Accessor vertexAccessor = model_.accessors[primitive.attributes.at("POSITION")];
		if (vertexAccessor.type != TINYGLTF_TYPE_VEC3 || vertexAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
		{
			ConsoleManager::PrintWarning("Component type is not float or data type is not vec3. Skipping primitive...");
			continue;
		}

		const uint32_t verticesInPrimitive = static_cast<uint32_t>(vertexAccessor.count);
		if (verticesInPrimitive + vCounter_ > totalVertexCount_)
		{
			ConsoleManager::PrintWarning("VertexCount from primitive will exceed allocated space. Skipping...");
			continue;
		}

		const tinygltf::BufferView vertexBufferView = model_.bufferViews[vertexAccessor.bufferView];
		const tinygltf::Buffer vertexBuffer = model_.buffers[vertexBufferView.buffer];

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

			// Apply transform & add value to vertexList
			newVertex.Pos = transform * newVertex.Pos;

			vertexList_[vCounter_ + i] = newVertex;
		}

		// Add indices based on vCounter_


		vCounter_ += verticesInPrimitive;
	}

	return error_const::SUCCESS;
}