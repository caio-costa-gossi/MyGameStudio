#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Enums.h"
#include "sqlite3.h"

struct AssetDependency
{
	uint32_t AssetId;
	uint32_t DependsOn;

	static AssetDependency FromStmt(sqlite3_stmt* stmt)
	{
		AssetDependency a;

		a.AssetId = sqlite3_column_int(stmt, 0);
		a.DependsOn = sqlite3_column_int(stmt, 1);

		return a;
	}
};

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

	static Asset FromStmt(sqlite3_stmt* stmt)
	{
		Asset a;

		a.Id = sqlite3_column_int(stmt, 0);
		a.Name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
		a.Extension = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
		a.Type = enums::StringToAssetType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
		a.SourceLocation = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
		a.AssetLocation = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
		a.LastModifiedDate = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));
		a.CheckModifications = sqlite3_column_int(stmt, 7);

		return a;
	}

	void AddDependencies(std::vector<AssetDependency> dependencies)
	{
		DependsOnAssets.reserve(64);

		for (auto it = dependencies.begin(); it != dependencies.end(); ++it)
		{
			DependsOnAssets.emplace_back(it->DependsOn);
		}
	}
};
