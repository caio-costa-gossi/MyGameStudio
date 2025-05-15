#pragma once
#include <cstdint>
#include <string>

#include "Table.h"

class AsciiDrawer
{
private:

public:
	static std::string DrawBox(uint8_t xSize, const char* text);
	static std::string DrawTable(Table<std::string>& data, uint32_t rightMargin = 2);
	static std::string MultiplyString(const std::string& string, uint32_t factor);

	static uint32_t GetMaxCharCount(const std::string* strings, const uint32_t stringCount);
	static std::string ParseEscape(const std::string& input);
	static bool IsLeadingUtf8Character(const unsigned char byte);
	static uint32_t CountUtf8Characters(const std::string& string);
	static std::string RightPadString(const std::string& string, const uint32_t charAmount, const std::string& paddingChar);
};
