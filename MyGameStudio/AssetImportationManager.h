#pragma once
#include "Asset.h"
#include "Err.h"

class AssetImportationManager
{
private:


public:
	static Err ImportAsset(const char* filepath);
	static Err DeleteAsset(const Asset& asset);
};
