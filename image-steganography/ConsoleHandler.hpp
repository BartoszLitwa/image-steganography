#pragma once
#include <string>
#include <iostream>

#include "src/Helpers.hpp"
#include "src/FileHandler.hpp"

class ConsoleHandler
{
private:
	const FileHandler* _handler;

	bool isSupportedFileFormat(std::string path);
	void handleInfoFlag();
	void handleEncodeFlag();
	void handleDecodeFlag();
	void handleCheckFlag();
	void handleHelpFlag();
	void handleUnknownFlag();
	void printMessage();
public:
	ConsoleHandler() : _handler(new FileHandler()) {}
	~ConsoleHandler() {}
	void handleConsoleInput(int argc, char* argv[]);
};