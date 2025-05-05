#include "MeshProcessor.h"
#include <sstream>

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

#include "ConsoleManager.h"
#include "LocalizationManager.h"

uint8_t* MeshProcessor::ProcessMesh(const Asset& metadata, uint64_t& resultSize, std::string& errMsg)
{
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;

	loader.LoadBinaryFromFile(&model, &errMsg, nullptr, metadata.SourceLocation);

	Err error = VerifyModel(model);
	if (error.Code())
	{
		errMsg = LocalizationManager::GetLocalizedString(string_const::G_NOT_TRIANGULATED);
		return nullptr;
	}

	Triangulate(model);
	CompressIndices(model);

	return GetFileBuffer(loader, model, resultSize);
}

uint8_t* MeshProcessor::GetFileBuffer(tinygltf::TinyGLTF& loader, const tinygltf::Model& model, uint64_t& resultSize)
{
	std::ostringstream outputStream;
	loader.WriteGltfSceneToStream(&model, outputStream, true, true);
	const std::string output = outputStream.str();

	resultSize = output.size();
	const auto resultBuffer = new uint8_t[resultSize];
	memcpy_s(resultBuffer, resultSize, output.data(), resultSize);

	return resultBuffer;
}

Err MeshProcessor::VerifyModel(const tinygltf::Model& model)
{
	for (const tinygltf::Mesh& mesh : model.meshes)
	{
		for (const tinygltf::Primitive& primitive : mesh.primitives)
			if (primitive.mode != 4 && primitive.mode != 5 && primitive.mode != 6)
				return error_const::MODEL_NOT_TRIANGULATED;
	}

	return error_const::SUCCESS;
}

Err MeshProcessor::Triangulate(tinygltf::Model& model)
{
	/*for (tinygltf::Mesh& mesh : model.meshes)
	{
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN)
			{
				primitive.
			}
		}
	}*/

	return error_const::SUCCESS;
}

Err MeshProcessor::CompressIndices(tinygltf::Model& model)
{
	for (tinygltf::Mesh& mesh : model.meshes)
	{
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			tinygltf::Accessor& accessor = model.accessors[primitive.indices];
			tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
			tinygltf::Buffer& buffer = model.buffers[view.buffer];

			size_t indexAmount = accessor.count;

			if (accessor.componentType != TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
				continue;

			const uint32_t* indices = reinterpret_cast<uint32_t*>(buffer.data.data() + view.byteOffset + accessor.byteOffset);

			uint32_t maxIndex = 0;
			for (size_t i = 0; i < indexAmount; ++i)
				maxIndex = std::max(maxIndex, indices[i]);

			int newComponentType;
			if (maxIndex < 255)
			{
				newComponentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
			}
			else if (maxIndex < 65535)
			{
				newComponentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT;
			}
			else continue;

			std::vector<unsigned char> compressedData;
			compressedData.reserve(indexAmount);

			if (newComponentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
			{
				for (size_t i = 0; i < indexAmount; ++i)
					compressedData.push_back(static_cast<uint8_t>(indices[i]));
			}
			else
			{
				compressedData.resize(indexAmount * sizeof(uint16_t));
				uint16_t* pCompressedData = reinterpret_cast<uint16_t*>(compressedData.data());

				for (size_t i = 0; i < indexAmount; ++i)
					pCompressedData[i] = static_cast<uint16_t>(indices[i]);
			}

			ChangeBuffer(buffer.data.data(), compressedData, view.byteOffset + accessor.byteOffset);
			accessor.componentType = newComponentType;
		}
	}

	return error_const::SUCCESS;
}

void MeshProcessor::ChangeBuffer(uint8_t* buffer, const std::vector<uint8_t>& newData, size_t initialOffset)
{
	size_t offset = 0;

	for (const uint8_t datum : newData)
	{
		buffer[initialOffset + offset] = datum;
		++offset;
	}
}
