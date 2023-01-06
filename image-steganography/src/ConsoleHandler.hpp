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

	/// <summary>
	/// Determines if the file path is to supported image file.
	/// </summary>
	/// <param name="path">Path to image</param>
	/// <returns></returns>
	bool isSupportedFileFormat(const std::string& path) const;
	/// <summary>
	/// Handles the Info Flag and prints the image info.
	/// </summary>
	void handleInfoFlag();
	/// <summary>
	/// Handles the Encode Flag and encodes the image with the message passed by user.
	/// </summary>
	/// <param name="msg"></param>
	void handleEncodeFlag(const std::string& msg);
	/// <summary>
	/// Handles the Decode Flag and decodes the image to retrieve the message encoded in Image.
	/// </summary>
	void handleDecodeFlag();
	/// <summary>
	/// Determines if the Image has been encoded with a message.
	/// </summary>
	/// <param name="msg"></param>
	void handleCheckFlag(const std::string& msg);
	/// <summary>
	/// Handles the Help Flag and prints the help message.
	/// </summary>
	void handleHelpFlag();
	/// <summary>
	/// Private helper for printing the messages on console depening on the message type.
	/// </summary>
	/// <param name="msg">Enum Message that determines which message should be displayed</param>
	/// <param name="arg">Optional string that could be displayed on specifi message</param>
	void printMessage(Messages msg, std::string arg = "") const;
public:
	/// <summary>
	/// Constructor
	/// </summary>
	ConsoleHandler() {
		_fileHandler = new FileHandler();
		_image = Image();
	}
	/// <summary>
	/// Destructor
	/// </summary>
	~ConsoleHandler() {
		_fileHandler->~FileHandler();
		delete _fileHandler;
	}
	/// <summary>
	/// Main method that handles the console input - flags
	/// </summary>
	/// <param name="argc">Number of arguments</param>
	/// <param name="argv">Arguments passed by user</param>
	void handleConsoleInput(int argc, char* argv[]);
};