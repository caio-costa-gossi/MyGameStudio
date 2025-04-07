#pragma once
#include <fstream>
#include <unordered_map>

class CsvParser
{
private:
	std::ifstream file_;

	std::unordered_map < const char*, std::vector<const char*> > data_;
	std::vector<const char*> header_;

	uint16_t columnAmount_;

	uint16_t ArrayIndexFrom2d(uint16_t row, uint16_t column) const;
	static std::vector<const char*> DivideString(const char* string);
	void InsertData(const std::vector<const char*>& list);
	void InitMap();

public:
	CsvParser(const char* fileName, const uint16_t columnAmount);
	~CsvParser() = default;
};
