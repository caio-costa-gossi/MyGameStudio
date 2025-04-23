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

	static Err CheckTables();

public:
	static Err Startup();
	static Err Shutdown();

	static std::vector<Asset> GetAssets();
	static Err RegisterAsset(const Asset& asset);
	static Err ModifyAsset(const Asset& asset);
};
