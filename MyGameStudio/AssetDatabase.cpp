#include "AssetDatabase.h"

#include "ConsoleManager.h"

Err AssetDatabase::Startup()
{
	ConsoleManager::Print("Opening asset database...\n", enums::ConsoleMessageType::info);
	Err error = db_.OpenDb(assetDbFilename_);

	if (error.Code())
		return error;

	ConsoleManager::Print("Checking/validating database tables...\n", enums::ConsoleMessageType::info);
	error = CheckTables();

	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::Shutdown()
{
	ConsoleManager::Print("Closing asset database...\n", enums::ConsoleMessageType::info);
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

Asset AssetDatabase::GetAsset(const uint32_t assetId, const bool addDependencies)
{
	auto asset = db_.ExecuteQuerySingle<Asset>(std::string("SELECT * FROM Assets WHERE Id = " + std::to_string(assetId)).c_str());

	if (addDependencies)
	{
		std::vector<AssetDependency> dependencies = db_.ExecuteQuery<AssetDependency>(std::string("SELECT * FROM AssetDependencies WHERE AssetId = " + std::to_string(assetId)).c_str());
		if (!dependencies.empty())
			asset.AddDependencies(dependencies);
	}

	return asset;
}

Err AssetDatabase::RegisterAsset(const Asset& asset)
{
	const std::string sqlStatement("INSERT INTO Assets (Name, Extension, Type, Size, SourceLocation, AssetLocation, LastModifiedDate, CheckModifications) VALUES ('" + 
		asset.Name + "','" +
		asset.Extension + "','" +
		enums::AssetTypeToString(asset.Type) + "'," +
		std::to_string(asset.Size) + ",'" +
		asset.SourceLocation + "','" +
		asset.AssetLocation + "','" +
		asset.LastModifiedDate + "'," +
		std::to_string(asset.CheckModifications) + ");"
	);

	const int64_t newAssetId64 = db_.ExecuteInsert(sqlStatement.c_str());
	if (newAssetId64 > INT32_MAX || newAssetId64 < INT32_MIN)
		return error_const::INTEGER_OUT_OF_BOUNDS;

	const int32_t newAssetId = static_cast<int32_t>(newAssetId64);
	if (newAssetId < 0)
	{
		ConsoleManager::Print("Error registering asset!\n", enums::ConsoleMessageType::error);
		return Err("Asset could not be registered", 10);
	}

	Err error = RegisterAssetDependencies(asset.DependsOnAssets, newAssetId);
	if (error.Code())
	{
		ConsoleManager::Print("Error registering asset dependencies!\n", enums::ConsoleMessageType::error);
		return error;
	}

	return error_const::SUCCESS;
}

Err AssetDatabase::UpdateAsset(const Asset& asset)
{
	const std::string sqlStatement("UPDATE Assets SET Name = '" + 
		asset.Name +
		"',Extension = '" + asset.Extension +
		"',Type = '" + enums::AssetTypeToString(asset.Type) +
		"',Size = " + std::to_string(asset.Size) +
		",SourceLocation = '" + asset.SourceLocation +
		"',AssetLocation = '" + asset.AssetLocation +	
		"',LastModifiedDate = '" + asset.LastModifiedDate +
		"',CheckModifications = " + std::to_string(asset.CheckModifications) +
		" WHERE Id = " + std::to_string(asset.Id));

	Err error = db_.ExecuteNonQuery(sqlStatement.c_str());
	if (error.Code())
	{
		ConsoleManager::Print("Error updating asset!\n", enums::ConsoleMessageType::error);
		return error;
	}

	error = DeleteAssetDependencies(asset.Id);
	if (error.Code())
	{
		ConsoleManager::Print("Error deleting asset dependencies before updating!\n", enums::ConsoleMessageType::error);
		return error;
	}

	error = RegisterAssetDependencies(asset.DependsOnAssets, asset.Id);
	if (error.Code())
	{
		ConsoleManager::Print("Error re-registering asset dependencies on update!\n", enums::ConsoleMessageType::error);
		return error;
	}

	return error_const::SUCCESS;
}

Err AssetDatabase::DeleteAsset(const uint32_t assetId)
{
	Err error = db_.ExecuteNonQuery(std::string("DELETE FROM Assets WHERE Id = " + std::to_string(assetId)).c_str());
	if (error.Code())
	{
		ConsoleManager::Print("Error deleting asset!\n", enums::ConsoleMessageType::error);
		return error;
	}

	error = DeleteAssetDependencies(assetId);
	if (error.Code())
	{
		ConsoleManager::Print("Error deleting dependencies!\n", enums::ConsoleMessageType::error);
		return error;
	}

	return error_const::SUCCESS;
}

Err AssetDatabase::ClearAllTables()
{
	Err error = db_.ExecuteNonQuery("DELETE FROM Assets;");
	if (error.Code())
	{
		ConsoleManager::Print("Error clearing Assets table!\n", enums::ConsoleMessageType::error);
		return error;
	}

	error = db_.ExecuteNonQuery("DELETE FROM AssetDependencies;");
	if (error.Code())
	{
		ConsoleManager::Print("Error clearing AssetDependencies table!\n", enums::ConsoleMessageType::error);
		return error;
	}

	return error_const::SUCCESS;
}

Err AssetDatabase::CheckTables()
{
	Err error = db_.ExecuteNonQuery(createAssetsTableQuery_);
	if (error.Code())
	{
		ConsoleManager::Print("Error checking/creating Asset table!\n", enums::ConsoleMessageType::error);
		return error;
	}

	error = db_.ExecuteNonQuery(createAssetDependenciesTableQuery_);
	if (error.Code())
	{
		ConsoleManager::Print("Error checking/creating AssetDependencies table!\n", enums::ConsoleMessageType::error);
		return error;
	}

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
		if (error.Code())
			ConsoleManager::Print("Error registering asset dependency!\n", enums::ConsoleMessageType::error);
	}

	return error_const::SUCCESS;
}


SqliteDatabase AssetDatabase::db_;
auto AssetDatabase::assetDbFilename_ = "Assets.db";

auto AssetDatabase::createAssetsTableQuery_ = 
"CREATE TABLE IF NOT EXISTS Assets "
"(Id INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT NOT NULL, Extension TEXT NOT NULL, "
"Type TEXT, Size INTEGER, SourceLocation TEXT, AssetLocation TEXT, LastModifiedDate TEXT, CheckModifications INTEGER);";

auto AssetDatabase::createAssetDependenciesTableQuery_ = 
"CREATE TABLE IF NOT EXISTS AssetDependencies "
"(AssetId INTEGER, DependsOn INTEGER, PRIMARY KEY(AssetId, DependsOn));";