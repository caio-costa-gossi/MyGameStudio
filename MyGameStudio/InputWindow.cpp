#include "InputWindow.h"
#include "ConsoleManager.h"
#include <iostream>
#include <limits>

std::string InputWindow::GetInput(const std::string& windowText, const std::string& options)
{
	ConsoleManager::PrintSimple(
		windowText + " " + options + ": ",
		false
	);

	std::string response;
	std::cin >> response;

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');

	return response;
}
