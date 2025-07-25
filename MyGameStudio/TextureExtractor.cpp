#include "TextureExtractor.h"

#include "ConsoleManager.h"

Err TextureExtractor::ProcessPrimitiveTexCoords(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, const uint32_t primitiveVertexCount)
{
	Err err = ProcessDiffuseCoords(model, primitive, info, primitiveVertexCount);
	if (err.Code())
		return err;

	return error_const::SUCCESS;
}

Err TextureExtractor::ProcessDiffuseCoords(const tinygltf::Model& model, const tinygltf::Primitive& primitive, const MeshAuxInfo& info, const uint32_t primitiveVertexCount)
{
	if (info.MapsInfo.DiffuseTexId < 0)
		return error_const::SUCCESS;

	if (primitive.attributes.find("TEXCOORD_" + std::to_string(info.MapsInfo.DiffuseTexCoordIndex)) == primitive.attributes.end())
	{
		ConsoleManager::PrintWarning("Tex coord attribute not found, but apparently mesh should have diffuse.");
		return error_const::SUCCESS;
	}

	const tinygltf::Accessor& accessor = model.accessors[primitive.attributes.at("TEXCOORD_" + std::to_string(info.MapsInfo.DiffuseTexCoordIndex))];

	if (accessor.count != primitiveVertexCount)
	{
		ConsoleManager::PrintError("Primitive TexCoord count is different than primitive vertex count. Skipping...");
		return error_const::SUCCESS;
	}

	if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
	{
		ConsoleManager::PrintWarning("Invalid texture coordinates component data type (" + std::to_string(accessor.componentType) + "). Skipping texture coordinates importation...");
		return error_const::SUCCESS;
	}

	if (accessor.type != TINYGLTF_TYPE_VEC2)
	{
		ConsoleManager::PrintWarning("Invalid texture coordinates data type (" + std::to_string(accessor.type) + "). Skipping texture coordinates importation...");
		return error_const::SUCCESS;
	}

	const tinygltf::BufferView bufferView = model.bufferViews[accessor.bufferView];
	const tinygltf::Buffer buffer = model.buffers[bufferView.buffer];

	const size_t texCoordCount = accessor.count;
	const size_t offset = accessor.byteOffset + bufferView.byteOffset;
	const size_t stride = bufferView.byteStride ? bufferView.byteStride : 2 * sizeof(float);

	const uint8_t* data = (buffer.data.data() + offset);

	for (size_t vertex = 0; vertex < texCoordCount; ++vertex)
	{
		const float* coordinates = reinterpret_cast<const float*>(&data[vertex * stride]);

		info.VertexList[info.VCounter + vertex].TexCoord.X = coordinates[0];
		info.VertexList[info.VCounter + vertex].TexCoord.Y = coordinates[1];
	}

	return error_const::SUCCESS;
}
