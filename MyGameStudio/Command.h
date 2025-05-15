#pragma once
#include "AsciiDrawer.h"
#include "AssetDatabase.h"
#include "AssetPipeline.h"
#include "ConsoleManager.h"
#include "Err.h"
#include "LocalizationManager.h"
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
		ConsoleManager::Print(std::string("\n") + LocalizationManager::GetLocalizedString(string_const::G_QUIT_CONSOLE) + "\n", enums::ConsoleMessageType::info);
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
			ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
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
			ConsoleManager::Print(err.Message(), enums::ConsoleMessageType::error);
			return error_const::SUCCESS;
		}

		return error_const::SUCCESS;
	}

	~ClearAssetDbCommand() override = default;
};

class ListAssetsCommand : public Command
{
private:
	static Err AssetVecToArray(const uint16_t colCount, std::string* targetData, const std::vector<Asset>& assets)
	{
		for (uint32_t assetIndex = 0; assetIndex < assets.size(); ++assetIndex)
		{
			targetData[assetIndex * colCount + 0] = std::to_string(assets[assetIndex].Id);
			targetData[assetIndex * colCount + 1] = assets[assetIndex].Name;
			targetData[assetIndex * colCount + 2] = assets[assetIndex].Extension;
			targetData[assetIndex * colCount + 3] = enums::AssetTypeToString(assets[assetIndex].Type);
			targetData[assetIndex * colCount + 4] = std::to_string(assets[assetIndex].SourceSize);
			targetData[assetIndex * colCount + 5] = std::to_string(assets[assetIndex].ProductSize);
			targetData[assetIndex * colCount + 6] = assets[assetIndex].SourceLocation;
			targetData[assetIndex * colCount + 7] = assets[assetIndex].ZipLocation + "\\" + assets[assetIndex].LocationInZip;
		}

		return error_const::SUCCESS;
	}

public:
	Err ExecuteCommand(uint8_t argc, char** argn, char** argv) override
	{
		const std::vector<Asset> assets = AssetDatabase::GetAssets();

		//const std::vector<Asset> assets = { Asset() };

		CsvParser parser("AssetTableCols.csv");
		const std::vector<std::unique_ptr<char[]>>& colNames = parser.GetColumn("names");
		const uint16_t colCount = static_cast<uint16_t>(colNames.size());

		auto valueData = new std::string[colCount * assets.size()];
		Err err = AssetVecToArray(colCount, valueData, assets);

		if (err.Code())
		{
			delete[] valueData;
			return err;
		}

		Table table(colNames, valueData, colCount, static_cast<uint32_t>(assets.size()));

		while (!table.IsTableEnd())
		{
			std::string printString;
			const std::string* row = table.GetNextRow();
			const std::vector<std::string>& colNames2 = table.GetColNames();

			for (uint32_t i = 0; i < table.ColCount(); ++i)
			{
				printString += colNames2[i] + ": " + row[i] + "\n";
			}

			ConsoleManager::PrintSimple(printString);
		}

		delete[] valueData;
		return error_const::SUCCESS;
	}

	~ListAssetsCommand() override = default;
};