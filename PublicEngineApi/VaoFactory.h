#pragma once
#include "Err.h"
#include "Mesh.h"

class VaoFactory
{
private:


public:
	static Err NewAttribObject(const Mesh& mesh, uint32_t& newVaoId);
	static Err NewBillboardAttribObject(uint32_t& newVaoId);
};
