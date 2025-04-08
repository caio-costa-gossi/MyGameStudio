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

	static std::vector<std::unique_ptr<char[]>> DivideString(const char* string);
	void InsertData(std::vector<std::unique_ptr<char[]>> list);
	void InitMap();

public:
	explicit CsvParser(const char* fileName);
	~CsvParser() = default;

	const char* GetValue(const char* column, uint16_t row);
	std::vector<std::unique_ptr<char[]>>& GetHeaders();
	std::vector<std::unique_ptr<char[]>>& GetColumn(const char* column);
};
