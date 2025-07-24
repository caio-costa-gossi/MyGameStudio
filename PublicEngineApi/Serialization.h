#pragma once
#include <cstdint>

#include "Mesh.h"
#include "Model.h"
#include "../MyGameStudio/DataStream.h"

namespace Serialization
{
	uint64_t CalculateMeshSize(const Mesh& mesh);
	DataStream SerializeMesh(const Mesh& mesh, uint64_t& resultSize);
	Mesh DesserializeMesh(const uint8_t* data, uint64_t dataSize);

	DataStream SerializeModel(const Model& model, uint64_t& resultSize);
	Model DesserializeModel(const uint8_t* data, uint64_t dataSize);
};