#include "MeshProcessor.h"
#include <sstream>

#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

uint8_t* MeshProcessor::ProcessMesh(const Asset& metadata, uint64_t& resultSize)
{
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string errorMsg;

	loader.LoadBinaryFromFile(&model, &errorMsg, nullptr, metadata.SourceLocation);

	std::ostringstream outputStream;
	loader.WriteGltfSceneToStream(&model, outputStream, true, true);
	const std::string output = outputStream.str();

	resultSize = output.size();
	const auto resultBuffer = new uint8_t[resultSize];
	memcpy_s(resultBuffer, resultSize, output.data(), resultSize);

	return resultBuffer;
}
