#pragma once
#include <string>
#include <iostream>

#include "Helpers.hpp"
#include "structs.hpp"
#include "FileHandler.hpp"

class ConsoleHandler
{
private:
	FileHandler* _fileHandler;
	std::string _filePath;
	Image _image;

	bool isSupportedFileFormat(const std::string& path) const;
	void handleInfoFlag();
	void handleEncodeFlag(const std::string& msg);
	void handleDecodeFlag();
	void handleCheckFlag(const std::string& msg);
	void handleHelpFlag();
	void printMessage(Messages msg, std::string arg = "") const;
public:
	ConsoleHandler() {
		_fileHandler = new FileHandler();
		_image = Image();
	}
	~ConsoleHandler() {
		_fileHandler->~FileHandler();
		delete _fileHandler;
	}
	void handleConsoleInput(int argc, char* argv[]);
};