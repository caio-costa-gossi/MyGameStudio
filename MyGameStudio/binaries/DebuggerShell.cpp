#include <iostream>
#include <string>
#include <Windows.h>

static uint32_t bytesLastWritten = 0;

static void ClearConsole(const uint32_t bytesToWrite)
{
	const HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hOutput, &csbi))
		return;

	if (bytesLastWritten != bytesToWrite)
	{
		DWORD charsWritten;
		FillConsoleOutputCharacterA(hOutput, ' ', 1024, {0,0}, &charsWritten);
		FillConsoleOutputAttribute(hOutput, csbi.wAttributes, 1024, {0,0}, &charsWritten);
	}

	SetConsoleCursorPosition(hOutput, { 0,0 });

	bytesLastWritten = bytesToWrite;
}

int main()
{
	std::cout << "Initializing debugger...\n";
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	char buffer[1024] = {0};
	uint32_t bytesRead;

	while (true)
	{
		ReadFile(hInput, buffer, sizeof(buffer) - 1, reinterpret_cast<LPDWORD>(&bytesRead), nullptr);
		ClearConsole(bytesRead);
		std::cout.write(buffer, bytesRead);
	}

	return 0;
}