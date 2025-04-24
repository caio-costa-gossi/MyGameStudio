#include "AssetDatabase.h"
#include "CommandFactory.h"
#include "ConsoleManager.h"
#include "EngineCoreManager.h"
#include "Err.h"
#include "SqliteDatabase.h"

int main()
{
	Err err = EngineCoreManager::Startup();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}

	err = EngineCoreManager::Config();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}

	//ConsoleManager::RunConsole();

	std::vector<uint32_t> dependencies = { 3,4 };
	//Asset asset{ 0, "myAsset.png", "png", enums::AssetType::image, "C:/Downloads/myAsset.png", "Assets/myAssetLegal.zip", "2025-02-01 00:00:00", false };
	Asset asset{ 0, "meuAsset.mat", "mat", enums::AssetType::material, "C:/Downloads/myMaterial.mat", "Assets/myAssetMaterial.zip", "2025-01-01 00:00:00", true };
	//Asset asset = AssetDatabase::GetAsset(1);
	//asset.DependsOnAssets = dependencies;

	AssetDatabase::DeleteAsset(0);

	err = EngineCoreManager::Shutdown();
	if (err.Code() != 0)
	{
		std::cout << "Error: " << err;
	}
}
