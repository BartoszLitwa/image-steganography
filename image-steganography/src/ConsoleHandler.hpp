#pragma once
#include <string>
#include <iostream>

#include "Helpers.hpp"
#include "FileHandler.hpp"

class ConsoleHandler
{
private:
	std::string filePath;
	std::string message;

	bool isSupportedFileFormat(std::string path);
	void handleInfoFlag();
	void handleEncodeFlag();
	void handleDecodeFlag();
	void handleCheckFlag();
	void handleHelpFlag();
	void handleUnknownFlag();
	void printMessage();
public:
	ConsoleHandler(int argc, char* argv[]);
};