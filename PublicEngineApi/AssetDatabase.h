#pragma once
#include <vector>
#include "Asset.h"
#include "SqliteDatabase.h"

class AssetDatabase
{
private:
	static SqliteDatabase db_;
	static const char* assetDbFilename_;
	static const char* createAssetsTableQuery_;
	static const char* createAssetDependenciesTableQuery_;
	static const char* createGameObjectTableQuery_;

	static Err CheckTables();
	static Err DeleteAssetDependencies(uint32_t assetId);
	static Err RegisterAssetDependencies(const std::vector<uint32_t>& dependencies, uint32_t assetId);

public:
	static Err Startup();
	static Err Shutdown();

	static std::vector<Asset> GetAssets();
	static Err GetAsset(uint32_t assetId, Asset& returnValue, bool addDependencies = true);
	static Err GetAssetBySrcLocation(const std::string& srcLocation, Asset& returnValue);
	static Err RegisterAsset(Asset& asset);
	static Err UpdateAsset(const Asset& asset);
	static Err DeleteAsset(uint32_t assetId);

	static Err ClearAllTables();
};
