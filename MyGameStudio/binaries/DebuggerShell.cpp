#include <iostream>
#include <string>
#include <Windows.h>

int main()
{
	std::cout << "Initializing debugger...\n";
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	char buffer[1024] = {0};
	uint32_t bytesRead;

	while (true)
	{
		ReadFile(hInput, buffer, sizeof(buffer) - 1, reinterpret_cast<LPDWORD>(&bytesRead), nullptr);
		std::cout.write(buffer, bytesRead);
	}

	return 0;
}