#pragma once
#include <cstdint>
#include <string>

class AsciiDrawer
{
private:

public:
	static std::string DrawBox(uint8_t xSize, const char* text);
	static std::string MultiplyString(const std::string& string, uint16_t factor);

	static std::string ParseEscape(const std::string& input);
	static bool IsLeadingUtf8Character(const unsigned char byte);
	static uint16_t CountUtf8Characters(const std::string& string);
	static std::string RightPadString(const std::string& string, const uint16_t charAmount, const std::string& paddingChar);
};
