#pragma once
#include <cstdint>
#include <string>

class AsciiDrawer
{
private:


public:
	static std::string DrawBox(uint8_t xSize, const char* text);
	static std::string MultiplyString(const std::string& string, uint8_t factor);

	static std::string ParseEscape(const std::string& input);
};
