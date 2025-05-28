#include "CsvParser.h"

#include <cassert>
#include <vector>
#include <cstring>
#include <iostream>

CsvParser::CsvParser(const char* fileName)
{
	data_ = std::unordered_map<const char*, std::vector<std::unique_ptr<char[]>>, CStrHash, CStrEqual>();

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
		std::vector<std::unique_ptr<char[]>> valueList = DivideString(buffer.data());
		InsertData(std::move(valueList));
		rowAmount++;
	}

	file_.close();
}

std::vector<std::unique_ptr<char[]>> CsvParser::DivideString(const char* string)
{
	auto returnList = std::vector<std::unique_ptr<char[]>>();

	char* nextToken = nullptr;
	auto editableString = std::unique_ptr<char[]>(new char[strlen(string) + 1]);

	strncpy_s(editableString.get(), strlen(string) + 1, string, _TRUNCATE);

	const char* token = strtok_s(editableString.get(), ",", &nextToken);
	while (token != nullptr)
	{
		auto smartToken = std::make_unique<char[]>(strlen(token) + 1);
		strncpy_s(smartToken.get(), strlen(token) + 1, token, _TRUNCATE);
		returnList.push_back(std::move(smartToken));

		token = strtok_s(nullptr, ",", &nextToken);
	}

	return returnList;
}

void CsvParser::InsertData(std::vector<std::unique_ptr<char[]>> list)
{
	for (int i = 0; i < header_.size(); ++i)
	{
		data_[header_[i].get()].push_back(std::move(list[i]));
	}
}

void CsvParser::InitMap()
{
	for (auto it = header_.begin(); it != header_.end(); ++it)
	{
		auto curString = (*it).get();
		data_.insert({ (*it).get(), std::vector<std::unique_ptr<char[]>>()});
	}
}

std::unique_ptr<char[]>& CsvParser::GetValue(const char* column, const uint16_t row)
{
	assert(row < data_[column].size());
	return data_[column][row];
}

std::vector<std::unique_ptr<char[]>>& CsvParser::GetHeaders()
{
	return header_;
}

std::vector<std::unique_ptr<char[]>>& CsvParser::GetColumn(const char* column)
{
	return data_[column];
}
