#include <iostream>
#include <string>
#include <Windows.h>

static void ClearConsole()
{
	const HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hOutput, &csbi))
		return;

	const DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	DWORD charsWritten;

	FillConsoleOutputCharacterA(hOutput, ' ', consoleSize, { 0,0 }, &charsWritten);
	FillConsoleOutputAttribute(hOutput, csbi.wAttributes, consoleSize, { 0,0 }, &charsWritten);
	SetConsoleCursorPosition(hOutput, { 0,0 });
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
		ClearConsole();
		std::cout.write(buffer, bytesRead);
	}

	return 0;
}