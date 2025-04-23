#include "AssetDatabase.h"

Err AssetDatabase::Startup()
{
	std::cout << "Opening asset database...\n";
	Err error = db_.OpenDb(assetDbFilename_);

	if (error.Code())
		return error;

	std::cout << "Checking/validating database tables...\n";
	error = CheckTables();

	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

Err AssetDatabase::Shutdown()
{
	std::cout << "Closing asset database...\n";
	Err error = db_.CloseDb();

	if (error.Code())
		return error;

	return error_const::SUCCESS;
}

std::vector<Asset> AssetDatabase::GetAssets()
{
	return db_.ExecuteQuery<Asset>("SELECT * FROM Assets;");
}

Err AssetDatabase::RegisterAsset(const Asset& asset)
{
	const std::string sqlStatement("INSERT INTO Assets VALUES (" + 
		std::to_string(asset.Id) + "," + 
		asset.Name + "," +
		asset.Extension + "," +
		enums::AssetTypeToString(asset.Type) + "," +
		asset.SourceLocation + "," +
		asset.AssetLocation + "," +
		asset.LastModifiedDate + "," +
		std::to_string(asset.CheckModifications) + ");"
	);

	Err error = db_.ExecuteNonQuery(sqlStatement.c_str());

	if (error.Code())
	{
		std::cout << "Error registering asset!";
		return error;
	}

	return error_const::SUCCESS;
}

Err AssetDatabase::ModifyAsset(const Asset& asset)
{
	const std::string sqlStatement("UPDATE Assets SET Name = " + 
		asset.Name +
		",Extension = " + asset.Extension +
		",Type = " + enums::AssetTypeToString(asset.Type) +
		",SourceLocation = " + asset.SourceLocation +
		",AssetLocation = " + asset.AssetLocation +	
		",LastModifiedDate = " + asset.LastModifiedDate +
		",CheckModifications = " + std::to_string(asset.CheckModifications) +
		" WHERE Id = " + std::to_string(asset.Id));

	Err error = db_.ExecuteNonQuery(sqlStatement.c_str());

	if (error.Code())
	{
		std::cout << "Error updating asset!";
		return error;
	}

	return error_const::SUCCESS;
}

Err AssetDatabase::CheckTables()
{
	Err error = db_.ExecuteNonQuery(createAssetsTableQuery_);

	if (error.Code())
	{
		std::cout << "Error checking/creating Asset table!\n";
		return error;
	}

	error = db_.ExecuteNonQuery(createAssetDependenciesTableQuery_);

	if (error.Code())
	{
		std::cout << "Error checking/creating AssetDependencies table!\n";
		return error;
	}

	return error_const::SUCCESS;
}

SqliteDatabase AssetDatabase::db_;
auto AssetDatabase::assetDbFilename_ = "Assets.db";

auto AssetDatabase::createAssetsTableQuery_ = 
"CREATE TABLE IF NOT EXISTS Assets "
"(Id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Extension TEXT NOT NULL, "
"Type TEXT, SourceLocation TEXT, AssetLocation TEXT, LastModifiedDate TEXT, CheckModifications INTEGER);";

auto AssetDatabase::createAssetDependenciesTableQuery_ = 
"CREATE TABLE IF NOT EXISTS AssetDependencies "
"(AssetId INTEGER, DependsOn INTEGER);";