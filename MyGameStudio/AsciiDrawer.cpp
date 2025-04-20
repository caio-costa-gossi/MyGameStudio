#include "AsciiDrawer.h"
#include <string>
#include <sstream>

std::string AsciiDrawer::DrawBox(const uint8_t xSize, const char* text)
{
	std::string returnString;
	std::stringstream textStream(ParseEscape(text));
	std::string textLine;

	returnString += "+" + MultiplyString("-", xSize*2) + "+\n";

	while (std::getline(textStream, textLine, '\n'))
	{
		textLine.resize(xSize * 2, ' ');
		returnString += "|" + textLine + "|\n";
	}

	returnString += "|" + MultiplyString(" ", xSize * 2) + "|\n";
	returnString += "+" + MultiplyString("-", xSize*2) + "+\n";

	return returnString;
}

std::string AsciiDrawer::MultiplyString(const std::string& string, const uint8_t factor)
{
	std::string returnValue;

	for (uint8_t i = 0; i < factor; ++i)
	{
		returnValue += string;
	}

	return returnValue;
}

std::string AsciiDrawer::ParseEscape(const std::string& input)
{
	std::string returnString;

	for (int i = 0; i < input.size(); ++i)
	{
		if (input[i] == '\\' && i + 1 < input.size() && input[i + 1] == 'n')
		{
			returnString += '\n';
			i++;
		}
		else
			returnString += input[i];
	}

	return returnString;
}