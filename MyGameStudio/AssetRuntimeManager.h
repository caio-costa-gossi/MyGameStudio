#pragma once
#include <unordered_map>

#include "Err.h"

class AssetRuntimeManager
{
private:
	static std::unordered_map<uint32_t, std::unique_ptr<uint8_t[]>> assetData_;

public:
	static Err Startup();
	static uint8_t* GetAssetData(uint32_t assetId);
};
