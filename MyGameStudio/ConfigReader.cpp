#include "ConfigReader.h"
#include <fstream>
#include "Err.h"

Err ConfigReader::LoadConfig(const char* fileName)
{
	configMap_ = std::unordered_map<const char*, std::unordered_map<const char*, std::unique_ptr<char[]>, CStrHash, CStrEqual>, CStrHash, CStrEqual>();

	std::ifstream file(fileName, std::ios::in);

	auto buffer = std::unique_ptr<char[]>(new char[256]);

	// Read the file
	const char* currentObject = nullptr;
	while (file.getline(buffer.get(), 256, '\n'))
	{
		if (buffer.get()[0] == ';')
			continue;

		if (buffer.get()[0] == '[' && buffer.get()[std::strlen(buffer.get()) - 1] == ']')
		{
			auto string = std::make_unique<char[]>(std::strlen(buffer.get()) - 1);
			strncpy_s(string.get(), std::strlen(buffer.get()) - 1, buffer.get() + 1, _TRUNCATE);

			stringStorage_.push_back(std::move(string));
			currentObject = stringStorage_.back().get();
		}
		else if (std::strlen(buffer.get()) > 0)
		{
			char* context = nullptr;

			const char* paramName = strtok_s(buffer.get(), "=", &context);
			const char* paramVal = strtok_s(nullptr, "=", &context);

			auto pParamName = std::make_unique<char[]>(std::strlen(paramName) + 1);
			strncpy_s(pParamName.get(), std::strlen(paramName) + 1, paramName, _TRUNCATE);
			stringStorage_.push_back(std::move(pParamName));

			auto pParamVal = std::make_unique<char[]>(std::strlen(paramVal) + 1);
			strncpy_s(pParamVal.get(), std::strlen(paramVal) + 1, paramVal, _TRUNCATE);

			configMap_[currentObject].insert({ stringStorage_.back().get(), std::move(pParamVal)});
		}
	}

	file.close();

	return error_const::SUCCESS;
}

const char* ConfigReader::GetConfig(const char* object, const char* config)
{
	return configMap_[object][config].get();
}

const std::unordered_map<const char*, std::unique_ptr<char[]>, CStrHash, CStrEqual>& ConfigReader::GetObjectConfig(const char* object)
{
	return configMap_[object];
}

//Err ReadFile()