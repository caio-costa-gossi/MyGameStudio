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
		textLine = RightPadString(textLine, xSize * 2, " ");
		returnString += "|" + textLine + "|\n";
	}

	returnString += "|" + MultiplyString(" ", xSize * 2) + "|\n";
	returnString += "+" + MultiplyString("-", xSize*2) + "+\n";

	return returnString;
}

std::string AsciiDrawer::DrawTable(Table<std::string>& data, const uint32_t rightMargin)
{
	if (data.IsRowMajor())
		return "Table must be column-major!";

	// Resolve column width
	const std::vector<std::string>& colNames = data.GetColNames();
	uint32_t* maxCharCount = new uint32_t[data.ColCount()];
	uint32_t totalTableWidth = 0;

	// Check spacing from data
	data.ResetCursor();
	while (!data.IsTableEnd())
	{
		const uint32_t colIndex = data.Current();
		maxCharCount[colIndex] = GetMaxCharCount(data.GetNext(), data.RowCount()) + rightMargin;
	}

	// Check spacing from headers
	for (uint32_t col = 0; col < data.ColCount(); ++col)
	{
		maxCharCount[col] = std::max(maxCharCount[col], CountUtf8Characters(colNames[col]) + rightMargin);
		totalTableWidth += maxCharCount[col];
	}
	totalTableWidth += data.ColCount() - 1;

	data.Transpose();

	// Format string
	std::string returnString;
	returnString += "+" + MultiplyString("-", totalTableWidth) + "+\n|";

	// Write headers
	for (uint32_t col = 0; col < data.ColCount(); ++col)
		returnString += RightPadString(colNames[col], maxCharCount[col], " ") + "|";

	returnString += "\n|" + MultiplyString("-", totalTableWidth) + "|";

	// Write data
	while (!data.IsTableEnd())
	{
		returnString += "\n|";
		const std::string* row = data.GetNext();

		for (uint32_t col = 0; col < data.ColCount(); ++col)
			returnString += RightPadString(row[col], maxCharCount[col], " ") + "|";
	}

	returnString += "\n+" + MultiplyString("-", totalTableWidth) + "+";

	delete[] maxCharCount;
	return returnString;
}

uint32_t AsciiDrawer::GetMaxCharCount(const std::string* strings, const uint32_t stringCount)
{
	uint32_t max = 0;

	for (uint32_t i = 0; i < stringCount; i++)
	{
		max = std::max(max, static_cast<uint32_t>(CountUtf8Characters(strings[i])));
	}

	return max;
}

std::string AsciiDrawer::MultiplyString(const std::string& string, const uint32_t factor)
{
	std::string returnValue;

	for (uint32_t i = 0; i < factor; ++i)
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

bool AsciiDrawer::IsLeadingUtf8Character(const unsigned char byte)
{
	return (byte & 0xC0) != 0x80;
}

uint32_t AsciiDrawer::CountUtf8Characters(const std::string& string)
{
	uint16_t charCount = 0;

	for (const unsigned char& c : string)
	{
		if (IsLeadingUtf8Character(c)) charCount++;
	}

	return charCount;
}

std::string AsciiDrawer::RightPadString(const std::string& string, const uint32_t charAmount, const std::string& paddingChar)
{
	std::string returnString = string;

	const uint32_t stringCharCount = CountUtf8Characters(string);

	if (stringCharCount >= charAmount) return string;

	const uint32_t needToPad = charAmount - stringCharCount;
	returnString += MultiplyString(paddingChar, needToPad);

	return returnString;
}
