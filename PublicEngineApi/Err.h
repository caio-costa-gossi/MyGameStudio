#pragma once
#include <string>
#include <iostream>

class Err
{
private:
	std::string errorMessage_;
	uint16_t errorCode_;

public:
	explicit Err(std::string errorMessage, uint16_t errorCode);

	std::string Message() const;
	uint16_t Code() const;

	friend std::ostream& operator<<(std::ostream& os, const Err& err);
};


namespace error_const
{
	const auto SUCCESS = Err("Success", 0);
	const auto GENERIC_EXCEPTION = Err("Undefined error", 1);
	const auto INVALID_FORMAT = Err("Format is invalid", 2);
	const auto SETTING_MISSING = Err("A configuration needed has not been set", 3);
	const auto INVALID_PARAMETERS = Err("Invalid parameters", 4);
	const auto REDUNDANT_CALL = Err("This operation has already been done", 5);
	const auto DB_CLOSED = Err("The operation cannot proceed, the database is closed", 6);
	const auto INTEGER_OUT_OF_BOUNDS = Err("Integer is out of bounds", 7);
	const auto PARAMETER_IS_NULL = Err("Parameter is null", 8);
	const auto BUFFER_OVERFLOW = Err("Buffer overflow", 9);
	const auto MODEL_NOT_TRIANGULATED = Err("3D Model is not triangulated", 20);
	const auto TIMELINE_ALREADY_STARTED = Err("Timeline was already started!", 30);
	const auto TIMELINE_IS_STOPPED = Err("Timeline is stopped!", 31);
	const auto TIMELINE_IS_PAUSED = Err("Timeline is paused!", 32);
	const auto TIMELINE_IS_RUNNING = Err("Timeline is running!", 33);
	const auto TIMELINE_IS_ASSOCIATED = Err("Timeline is associated!", 34);
	const auto TIMELINE_IS_NOT_ASSOCIATED = Err("Timeline is not associated!", 35);
	const auto CALLBACK_UNSUCCESSFUL = Err("Callback was not executed successfully", 60);
	const auto COMMAND_UNSUCCESSFUL = Err("Command did not execute successfully", 70);
	const auto CMAKE_NOT_PRESENT = Err("cmake.exe not found! Please install it and add the relative path to config.ini", 80);
	const auto CMAKE_TEMPLATE_NOT_FOUND = Err("File 'CMakeTemplate.txt' was not found. Please make sure it is in the same folder as the game source code.", 81);
	const auto GAME_ALREADY_RUNNING = Err("The game is already running!", 90);
	const auto GAME_NOT_RUNNING = Err("The game is not running yet!", 91);
	const auto GAME_BIN_NOT_FOUND = Err("The game binary was not found!", 92);
	const auto GAME_DEBUGGER_ALREADY_RUNNING = Err("The game debugger is already running!", 93);
	const auto GAME_DEBUGGER_NOT_RUNNING = Err("The game debugger is not running!", 94);
	const auto ERROR_OPEN_FILE = Err("Error opening file", 100);
	const auto TERMINAL_OPEN_FAILURE = Err("Terminal was not created successfully", 110);
	const auto TERMINAL_PIPE_OPEN_FAILURE = Err("Terminal communication pipeline was not created successfully", 111);

	constexpr auto DB_ERROR_CODE = 10;
	constexpr auto ZIP_ERROR_CODE = 11;
	constexpr auto ASSET_IMPORTATION_ERROR_CODE = 12;
}