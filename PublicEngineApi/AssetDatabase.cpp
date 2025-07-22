#include "AssetDatabase.h"
#include "LocalizationManager.h"
#include "../MyGameStudio/ConsoleManager.h"

Err AssetDatabase::Startup()
{
	Err error = db_.OpenDb(assetDbFilename_);

	if (error.Code())
		return error;

	error = CheckTables();

	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::Shutdown()
{
	Err error = db_.CloseDb();

	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

std::vector<Asset> AssetDatabase::GetAssets()
{
	std::vector<Asset> assets = db_.ExecuteQuery<Asset>("SELECT * FROM Assets;");

	for (auto it = assets.begin(); it != assets.end(); ++it)
	{
		std::string queryDependencies("SELECT * FROM AssetDependencies WHERE AssetId = " + std::to_string(it->Id));
		std::vector<AssetDependency> dependencies = db_.ExecuteQuery<AssetDependency>(queryDependencies.c_str());

		if (!dependencies.empty())
			it->AddDependencies(dependencies);
	}

	return assets;
}

Err AssetDatabase::GetAsset(const uint32_t assetId, Asset& returnValue, const bool addDependencies)
{
	Asset asset;
	Err err = db_.ExecuteQuerySingle<Asset>(std::string("SELECT * FROM Assets WHERE Id = " + std::to_string(assetId)).c_str(), asset);
	if (err.Code())
		return err;

	if (addDependencies)
	{
		std::vector<AssetDependency> dependencies = db_.ExecuteQuery<AssetDependency>(std::string("SELECT * FROM AssetDependencies WHERE AssetId = " + std::to_string(assetId)).c_str());
		if (!dependencies.empty())
			asset.AddDependencies(dependencies);
	}

	returnValue = asset;
	return error_const::SUCCESS;
}

Err AssetDatabase::GetAssetBySrcLocation(const std::string& srcLocation, Asset& returnValue)
{
	const std::vector<Asset> foundAssets = db_.ExecuteQuery<Asset>(std::string("SELECT * FROM Assets WHERE SourceLocation = '" + srcLocation + "';").c_str());

	if (foundAssets.empty())
		return error_const::ASSET_NOT_FOUND;

	returnValue = foundAssets[0];

	return error_const::SUCCESS;
}

Err AssetDatabase::RegisterAsset(Asset& asset)
{
	// Check if asset already exists
	Asset existingAsset;
	Err err = GetAssetBySrcLocation(asset.SourceLocation, existingAsset);
	if (err.Code() == 0)
	{
		ConsoleManager::PrintWarning("Asset '" + asset.Name + "' was already imported before from source '" + existingAsset.SourceLocation + "'. Overwritting...");

		asset.Id = existingAsset.Id;
		err = UpdateAsset(asset);
		if (err.Code())
			return err;

		return error_const::SUCCESS;
	}

	// Register
	const std::string sqlStatement("INSERT INTO Assets (Name, Extension, Type, SourceSize, ProductSize, SourceLocation, ZipLocation, AssetLocation, LastModifiedDate, CheckModifications) VALUES ('" + 
		asset.Name + "','" +
		asset.Extension + "','" +
		enums::AssetTypeToString(asset.Type) + "'," +
		std::to_string(asset.SourceSize) + "," +
		std::to_string(asset.ProductSize) + ",'" +
		asset.SourceLocation + "','" +
		asset.ZipLocation + "','" +
		asset.AssetLocation + "','" +
		asset.LastModifiedDate + "'," +
		std::to_string(asset.CheckModifications) + ");"
	);

	int64_t newAssetId64;
	Err error = db_.ExecuteInsert(sqlStatement.c_str(), newAssetId64);
	if (error.Code())
		return error;

	if (newAssetId64 > INT32_MAX || newAssetId64 < INT32_MIN)
		return error_const::INTEGER_OUT_OF_BOUNDS;

	const int32_t newAssetId = static_cast<int32_t>(newAssetId64);
	asset.Id = newAssetId;

	error = RegisterAssetDependencies(asset.DependsOnAssets, newAssetId);
	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::UpdateAsset(const Asset& asset)
{
	const std::string sqlStatement("UPDATE Assets SET Name = '" + 
		asset.Name +
		"',Extension = '" + asset.Extension +
		"',Type = '" + enums::AssetTypeToString(asset.Type) +
		"',SourceSize = " + std::to_string(asset.SourceSize) +
		",ProductSize = " + std::to_string(asset.ProductSize) +
		",SourceLocation = '" + asset.SourceLocation +
		"',ZipLocation = '" + asset.ZipLocation +
		"',AssetLocation = '" + asset.AssetLocation +
		"',LastModifiedDate = '" + asset.LastModifiedDate +
		"',CheckModifications = " + std::to_string(asset.CheckModifications) +
		" WHERE Id = " + std::to_string(asset.Id));

	Err error = db_.ExecuteNonQuery(sqlStatement.c_str());
	if (error.Code())
		return error;

	error = DeleteAssetDependencies(asset.Id);
	if (error.Code())
		return error;

	error = RegisterAssetDependencies(asset.DependsOnAssets, asset.Id);
	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::DeleteAsset(const uint32_t assetId)
{
	Err error = db_.ExecuteNonQuery(std::string("DELETE FROM Assets WHERE Id = " + std::to_string(assetId)).c_str());
	if (error.Code())
		return error;

	error = DeleteAssetDependencies(assetId);
	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::ClearAllTables()
{
	Err error = db_.ExecuteNonQuery("DELETE FROM Assets;");
	if (error.Code())
		return error;

	error = db_.ExecuteNonQuery("DELETE FROM AssetDependencies;");
	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::CheckTables()
{
	Err error = db_.ExecuteNonQuery(createAssetsTableQuery_);
	if (error.Code())
		return error;

	error = db_.ExecuteNonQuery(createAssetDependenciesTableQuery_);
	if (error.Code())
		return error;

	error = db_.ExecuteNonQuery(createGameObjectTableQuery_);
	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::DeleteAssetDependencies(uint32_t assetId)
{
	Err error = db_.ExecuteNonQuery(std::string("DELETE FROM AssetDependencies WHERE AssetId = " + std::to_string(assetId)).c_str());
	if (error.Code()) return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::RegisterAssetDependencies(const std::vector<uint32_t>& dependencies, uint32_t assetId)
{
	if (dependencies.empty())
		return error_const::SUCCESS;

	for (auto it = dependencies.begin(); it != dependencies.end(); ++it)
	{
		Err error = db_.ExecuteNonQuery(std::string("INSERT INTO AssetDependencies VALUES (" + std::to_string(assetId) + "," + std::to_string(*it) + ");").c_str());
	}

	return error_const::SUCCESS;
}


SqliteDatabase AssetDatabase::db_;
auto AssetDatabase::assetDbFilename_ = "Assets.db";

auto AssetDatabase::createAssetsTableQuery_ = 
"CREATE TABLE IF NOT EXISTS Assets "
"(Id INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT NOT NULL, Extension TEXT NOT NULL, "
"Type TEXT, SourceSize INTEGER, ProductSize INTEGER, SourceLocation TEXT UNIQUE, ZipLocation TEXT, AssetLocation TEXT, LastModifiedDate TEXT, CheckModifications INTEGER);";

auto AssetDatabase::createAssetDependenciesTableQuery_ = 
"CREATE TABLE IF NOT EXISTS AssetDependencies "
"(AssetId INTEGER, DependsOn INTEGER, PRIMARY KEY(AssetId, DependsOn));";

auto AssetDatabase::createGameObjectTableQuery_ =
"CREATE TABLE IF NOT EXISTS GameObjects "
"(GameObjectId INTEGER PRIMARY KEY AUTOINCREMENT, ScriptPath TEXT, MeshAssetId INTEGER, TextureAssetId INTEGER);";