#pragma once
#include <fstream>
#include <unordered_map>

struct CStrHash {
	std::size_t operator()(const char* s) const {
		return std::hash<std::string>()(s);
	}
};

struct CStrEqual {
	bool operator()(const char* a, const char* b) const {
		return std::strcmp(a, b) == 0;
	}
};

class CsvParser
{
private:
	std::ifstream file_;

	std::unordered_map < const char*, std::vector<std::unique_ptr<char[]>>, CStrHash, CStrEqual > data_;
	std::vector<std::unique_ptr<char[]>> header_;

	uint16_t columnAmount_;

	uint16_t ArrayIndexFrom2d(uint16_t row, uint16_t column) const;
	static std::vector<std::unique_ptr<char[]>> DivideString(const char* string);
	void InsertData(std::vector<std::unique_ptr<char[]>> list);
	void InitMap();

public:
	CsvParser(const char* fileName, const uint16_t columnAmount);
	~CsvParser() = default;

	const char* GetValue(const char* column, uint16_t row);
};
