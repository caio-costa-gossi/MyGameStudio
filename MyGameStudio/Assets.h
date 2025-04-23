#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Enums.h"

struct Asset
{
	uint32_t Id;
	std::string Name;
	std::string Extension;
	enums::AssetType Type;
	std::string SourceLocation;
	std::string AssetLocation;
	std::string LastModifiedDate;
	bool CheckModifications = false;
	std::vector<uint32_t> DependsOnAssets;
};