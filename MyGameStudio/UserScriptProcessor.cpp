#include "UserScriptProcessor.h"
#include <fstream>
#include "ConsoleManager.h"
#include "StringUtils.h"
#include "SystemPathHelper.h"

uint8_t* UserScriptProcessor::ProcessScript(const Asset& metadata, uint64_t& resultSize)
{
	ConsoleManager::PrintInfo("Importing script...");

	std::string registerTxtFormatted = registerClassText_;
	StringUtils::ReplaceInString(registerTxtFormatted, "@class_name", SystemPathHelper::RemoveFileExtension(metadata.Name));

	resultSize = metadata.SourceSize + registerTxtFormatted.size();

	const auto scriptBuffer = new uint8_t[resultSize];
	LoadFile(metadata.SourceLocation, scriptBuffer, resultSize);

	memcpy_s(scriptBuffer + metadata.SourceSize, registerTxtFormatted.size(), registerTxtFormatted.c_str(), registerTxtFormatted.size());

	ConsoleManager::PrintInfo("Complete!");
	return scriptBuffer;
}

uint8_t* UserScriptProcessor::ProcessHeader(const Asset& metadata, uint64_t& resultSize)
{
	ConsoleManager::PrintInfo("Importing header...");

	resultSize = metadata.SourceSize;
	const auto scriptBuffer = new uint8_t[resultSize];
	LoadFile(metadata.SourceLocation, scriptBuffer, resultSize);

	ConsoleManager::PrintInfo("Complete!");
	return scriptBuffer;
}

Err UserScriptProcessor::LoadFile(const std::string& filepath, uint8_t* fileBuffer, const uint64_t bufferSize)
{
	std::ifstream file(filepath, std::ios::binary);

	if (!file.is_open())
		return error_const::ERROR_OPEN_FILE;

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(fileBuffer), static_cast<uint32_t>(bufferSize));

	return error_const::SUCCESS;
}

std::string UserScriptProcessor::registerClassText_ =
"\r\n#include \"GameObjectMacro.h\"\r\n#include \"GameObjectManager.h\"\r\nREGISTER_GAME_OBJECT(@class_name)";