#include "MeshProcessor.h"
#include <sstream>

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

#include "ConsoleManager.h"

uint8_t* MeshProcessor::ProcessMesh(const Asset& metadata, uint64_t& resultSize)
{
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string errorMsg;

	loader.LoadBinaryFromFile(&model, &errorMsg, nullptr, metadata.SourceLocation);

	CompressIndices(model);

	std::ostringstream outputStream;
	loader.WriteGltfSceneToStream(&model, outputStream, true, true);
	const std::string output = outputStream.str();

	resultSize = output.size();
	const auto resultBuffer = new uint8_t[resultSize];
	memcpy_s(resultBuffer, resultSize, output.data(), resultSize);

	return resultBuffer;
}

Err MeshProcessor::CompressIndices(tinygltf::Model& model)
{
	for (tinygltf::Mesh& mesh : model.meshes)
	{
		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			const tinygltf::Accessor accessor = model.accessors[primitive.indices];
			tinygltf::BufferView view = model.bufferViews[accessor.bufferView];
			tinygltf::Buffer buffer = model.buffers[view.buffer];

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

			tinygltf::Buffer newBuffer;
			newBuffer.data = compressedData;
			newBuffer.uri.clear();
			model.buffers.push_back(std::move(newBuffer));

			tinygltf::BufferView newView;
			newView.buffer = static_cast<int>(model.buffers.size() - 1);
			newView.byteOffset = 0;
			newView.byteLength = compressedData.size();
			newView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
			model.bufferViews.push_back(std::move(newView));

			tinygltf::Accessor newAccessor;
			newAccessor.bufferView = static_cast<int>(model.bufferViews.size() - 1);
			newAccessor.componentType = newComponentType;
			newAccessor.count = indexAmount;
			newAccessor.type = accessor.type;
			model.accessors.push_back(std::move(newAccessor));

			primitive.indices = static_cast<int>(model.accessors.size() - 1);
		}
	}

	return error_const::SUCCESS;
}
