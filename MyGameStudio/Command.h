#pragma once
#include "AsciiDrawer.h"
#include "AssetDatabase.h"
#include "AssetPipeline.h"
#include "ConsoleManager.h"
#include "Err.h"
#include "GameBuilder.h"
#include "GameRuntimeTestManager.h"
#include "LocalizationManager.h"
#include "SystemPathHelper.h"
#include "Table.h"

class Command
{
protected:
	static bool ValidateArgs(uint8_t argc, char** argn, char** argv, uint8_t expectedArgc, int intArgs[] = nullptr, float floatArgs[] = nullptr);
	static const char* GetArg(uint8_t argc, char** argn, char** argv, const char* argName);
	static void PrintUsage(const char* commandName);

public:
	virtual Err ExecuteCommand(uint8_t argc, char** argn, char** argv) = 0;
	virtual ~Command() = default;
};

class NotFoundCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		std::cout << "\nCommand not found...\n";
		return error_const::SUCCESS;
	}

	~NotFoundCommand() override = default;
};

class HelpCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		//ValidateArgs(argc, argn, argv, 1);

		std::cout << AsciiDrawer::DrawBox(30, LocalizationManager::GetLocalizedString(string_const::G_HELP_COMMAND));
		return error_const::SUCCESS;
	}

	~HelpCommand() override = default;
};

class QuitCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_QUIT_CONSOLE));
		ConsoleManager::StopConsole();
		return error_const::SUCCESS;
	}

	~QuitCommand() override = default;
};

class ChangeLangCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		const char* lang = GetArg(argc, argn, argv, "lang");

		if (!ValidateArgs(argc, argn, argv, 1) || lang == nullptr)
		{
			PrintUsage(string_const::G_USE_CHLANG);
			return error_const::SUCCESS;
		}

		if (enums::StringToLanguage(lang) == enums::Language::lang_not_found)
		{
			ConsoleManager::PrintSimple(LocalizationManager::GetLocalizedString(string_const::G_LANGUAGE_NOT_FOUND) + std::string(lang));
			return error_const::SUCCESS;
		}

		LocalizationManager::SetLanguage(enums::StringToLanguage(lang));

		std::cout << LocalizationManager::GetLocalizedString(string_const::G_LANGUAGE_CHANGED) << lang << "\n";
		return error_const::SUCCESS;
	}

	~ChangeLangCommand() override = default;
};

class ImportAssetCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		const char* filepath = GetArg(argc, argn, argv, "file");

		if (!ValidateArgs(argc, argn, argv, 1) || filepath == nullptr)
		{
			PrintUsage(string_const::G_USE_IMPORT);
			return error_const::SUCCESS;
		}

		Err err = AssetPipeline::ImportAsset(filepath);
		if (err.Code())
		{
			ConsoleManager::PrintError(err.Message());
			return error_const::SUCCESS;
		}

		return error_const::SUCCESS;
	}

	~ImportAssetCommand() override = default;
};

class ClearAssetDbCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		Err err = AssetDatabase::ClearAllTables();
		if (err.Code())
		{
			ConsoleManager::PrintError(err.Message());
			return error_const::SUCCESS;
		}

		return error_const::SUCCESS;
	}

	~ClearAssetDbCommand() override = default;
};

class ListAssetsCommand : public Command
{
private:
	static Err AssetVecToArray(const uint32_t rowCount, std::string* targetData, const std::vector<Asset>& assets)
	{
		for (uint32_t assetIndex = 0; assetIndex < assets.size(); ++assetIndex)
		{
			targetData[0 * rowCount + assetIndex] = std::to_string(assets[assetIndex].Id);
			targetData[1 * rowCount + assetIndex] = assets[assetIndex].Name;
			targetData[2 * rowCount + assetIndex] = assets[assetIndex].Extension;
			targetData[3 * rowCount + assetIndex] = enums::AssetTypeToString(assets[assetIndex].Type);
			targetData[4 * rowCount + assetIndex] = std::to_string(assets[assetIndex].SourceSize);
			targetData[5 * rowCount + assetIndex] = std::to_string(assets[assetIndex].ProductSize);
			targetData[6 * rowCount + assetIndex] = assets[assetIndex].SourceLocation;
			targetData[7 * rowCount + assetIndex] = assets[assetIndex].ZipLocation + SystemPathHelper::GetUnixSeparator() + assets[assetIndex].AssetLocation;
		}

		return error_const::SUCCESS;
	}

public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		const std::vector<Asset> assets = AssetDatabase::GetAssets();

		CsvParser parser("AssetTableCols.csv");
		const std::vector<std::unique_ptr<char[]>>& colNames = parser.GetColumn("names");
		const uint16_t colCount = static_cast<uint16_t>(colNames.size());
		const uint32_t rowCount = static_cast<uint32_t>(assets.size());

		auto valueData = new std::string[colCount * assets.size()];
		Err err = AssetVecToArray(rowCount, valueData, assets);

		if (err.Code())
		{
			delete[] valueData;
			return err;
		}

		Table table(colNames, valueData, colCount, rowCount, false);
		ConsoleManager::PrintSimple(AsciiDrawer::DrawTable(table));

		delete[] valueData;
		return error_const::SUCCESS;
	}

	~ListAssetsCommand() override = default;
};

class RunGameCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		Err err = GameRuntimeTestManager::RunGame();
		if (err.Code())
			ConsoleManager::PrintError(err.Message());

		return error_const::SUCCESS;
	}

	~RunGameCommand() override = default;
};

class QuitGameCommand : public Command
{
public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		ConsoleManager::PrintInfo(LocalizationManager::GetLocalizedString(string_const::G_QUIT_GAME));

		Err err = GameRuntimeTestManager::QuitGame();
		if (err.Code())
			ConsoleManager::PrintError(err.Message());

		return error_const::SUCCESS;
	}

	~QuitGameCommand() override = default;
};