#include "UserScriptProcessor.h"
#include <fstream>
#include "ConsoleManager.h"

uint8_t* UserScriptProcessor::ProcessScript(const Asset& metadata, uint64_t& resultSize)
{
	ConsoleManager::PrintInfo("Importing script...");

	resultSize = metadata.SourceSize + 20;

	const auto scriptBuffer = new uint8_t[resultSize];
	LoadFile(metadata.SourceLocation, scriptBuffer, resultSize);

	std::string append = "\n apple";
	memcpy_s(scriptBuffer + metadata.SourceSize, 20, append.c_str(), append.size());

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
