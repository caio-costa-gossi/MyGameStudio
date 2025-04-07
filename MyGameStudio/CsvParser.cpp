#include "CsvParser.h"

#include <vector>
#include <cstring>

CsvParser::CsvParser(const char* fileName, const uint16_t columnAmount): columnAmount_(columnAmount)
{
	data_ = std::unordered_map<const char*, std::vector<const char*>>();

	file_.open(fileName, std::ios_base::in);

	auto buffer = std::vector<char>(1024);

	// Adjust buffer size
	do
	{
		buffer.resize(buffer.size() * 2);
		file_.getline(buffer.data(), buffer.size());
		file_.clear();
		file_.seekg(0, std::ios_base::beg);
	}
	while (file_.gcount() == buffer.size() - 1);

	// Get header
	file_.getline(buffer.data(), buffer.size());
	header_ = DivideString(buffer.data());
	InitMap();

	// Get values
	uint16_t rowAmount = 0;
	while (file_.getline(buffer.data(), buffer.size()))
	{
		std::vector<const char*> valueList = DivideString(buffer.data());
		InsertData(valueList);
		rowAmount++;
	}
}

std::vector<const char*> CsvParser::DivideString(const char* string)
{
	auto returnList = std::vector<const char*>();

	char* nextToken = nullptr;
	char* editableString = nullptr;

	strncpy_s(editableString, sizeof(string), string, _TRUNCATE);

	const char* token = strtok_s(editableString, ",", &nextToken);
	while (token != nullptr)
	{
		returnList.push_back(token);
		token = strtok_s(editableString, ",", &nextToken);
	}

	return returnList;
}

void CsvParser::InsertData(const std::vector<const char*>& list)
{
	for (int i = 0; i < header_.size(); ++i)
	{
		data_[header_[i]].push_back(list[i]);
	}
}

void CsvParser::InitMap()
{
	for (auto it = header_.begin(); it != header_.end(); ++it)
	{
		data_.insert({ *it, std::vector<const char*>() });
	}
}

uint16_t CsvParser::ArrayIndexFrom2d(const uint16_t row, const uint16_t column) const
{
	return columnAmount_ * row + column;
}
