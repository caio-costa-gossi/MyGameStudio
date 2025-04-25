#pragma once
#include "Asset.h"
#include "Err.h"

class AssetPipeline
{
private:
	static Asset GetAssetMetadata();

public:
	static Err ImportAsset(const char* filepath);
};
