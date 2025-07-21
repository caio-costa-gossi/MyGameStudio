#pragma once
#include <cstdint>

#include "Mesh.h"
#include "../MyGameStudio/DataStream.h"

namespace Serialization
{
	DataStream SerializeMesh(const Mesh& mesh);
	Mesh DesserializeMesh(uint8_t* data);
};