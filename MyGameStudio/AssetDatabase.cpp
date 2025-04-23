#include "AssetDatabase.h"

Err AssetDatabase::Startup()
{
	Err error = db_.OpenDb(assetDbFilename_);

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
	return std::vector<Asset>();
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
	const std::string sqlStatement("SELECT");

	return error_const::SUCCESS;
}

Err AssetDatabase::CheckTables()
{
	return error_const::SUCCESS;
}

SqliteDatabase AssetDatabase::db_;
auto AssetDatabase::assetDbFilename_ = "Assets.db";