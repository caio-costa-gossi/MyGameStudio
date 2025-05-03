#pragma once
#include <cstdint>
#include "Asset.h"

class MeshProcessor
{
private:


public:
	static uint8_t* ProcessMesh(const Asset& metadata, uint64_t& resultSize);
};
