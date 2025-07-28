#pragma once
#include "BillboardData.h"
#include "Err.h"
#include "Mesh.h"

class VaoFactory
{
private:


public:
	static Err NewAttribObject(const Mesh& mesh, uint32_t& newVaoId);
	static Err NewBillboardAttribObject(const BillboardData& data, uint32_t& newVaoId);
};
