#pragma once
#include "ConsoleHandler.hpp"

/// <summary>
/// Determines if the file path is to supported image file.
/// </summary>
/// <param name="path">Path to image</param>
/// <returns></returns>
bool ConsoleHandler::isSupportedFileFormat(const std::string& path) const {
    if (path.length() == 0) {
        printMessage(Messages::MSG_MISSING_FILEPATH);
		return false;
    }
	
    // Return true if the file at the given path has a supported format .ppm or .bmp, false otherwise
    return Helpers::endsWith(path, ".ppm") || Helpers::endsWith(path, ".bmp");
}

/// <summary>
/// Handles the Info Flag and prints the image info.
/// </summary>
void ConsoleHandler::handleInfoFlag() {
    if (!isSupportedFileFormat(_filePath)) {
        printMessage(Messages::MSG_UNSUPPORTED_FILE_FROMAT);
        return;
    }
	
    if (!_fileHandler->getInfoImage(_filePath, _image)) {
		printMessage(Messages::MSG_UNABLE_TO_READ);
		return;
    }

    // Display information about the file at filePath, such as its size, memory usage, and last modification timestamp from _image
	std::cout << "File path: " << _filePath << std::endl;
	std::cout << "File format: " << fileTypeToString.at(_image.fileType) << std::endl;
	std::cout << "File size: " << (float)_image.fileSize / 1024 / 1024 << " MB (" << _image.fileSize << " B)" << std::endl;
	std::cout << "Width: " << _image.width << " Height: " << _image.height << std::endl;
	std::cout << "Pixels: " << _image.width * _image.height << std::endl;
    std::cout << "Bits per Pixel: " << _image.bitsPerPixel << std::endl;
    std::cout << "Last Modified Time: " << _image.last_modified_time << std::endl;
}

/// <summary>
/// Handles the Encode Flag and encodes the image with the message passed by user.
/// </summary>
/// <param name="msg"></param>
void ConsoleHandler::handleEncodeFlag(const std::string& msg) {
    if (!isSupportedFileFormat(_filePath)) {
        printMessage(Messages::MSG_UNSUPPORTED_FILE_FROMAT);
        return;
    }

    // Open the file at filePath and encode the message into it
    if (!_fileHandler->checkIfCanWrite(_filePath, msg)) {
        printMessage(Messages::MSG_UNABLE_TO_WRITE);
        return;
    }

    if (!_fileHandler->encodeMessage(_filePath, msg)) {
		printMessage(Messages::MSG_UNABLE_TO_ENCODE);
		return;
    }

    std::cout << "Successfully encoded message:\n" << msg << std::endl;
}

/// <summary>
/// Handles the Decode Flag and decodes the image to retrieve the message encoded in Image.
/// </summary>
void ConsoleHandler::handleDecodeFlag() {
    if (!isSupportedFileFormat(_filePath)) {
        printMessage(Messages::MSG_UNSUPPORTED_FILE_FROMAT);
        std::cout << "Error: unsupported file format" << std::endl;
        return;
    }

    // Open the file at filePath and decode any message stored in it
    if (!_fileHandler->checkIfCanRead(_filePath)) {
        printMessage(Messages::MSG_UNABLE_TO_READ);
        return;
    }

    std::string msg = _fileHandler->decodeMessage(_filePath);
    if (msg.empty()) {
        printMessage(Messages::MSG_UNABLE_TO_DECODE);
        return;
    }
	
	std::cout << "Successfully Decoded message:\n" << msg << std::endl;
}

/// <summary>
/// Determines if the Image has been encoded with a message.
/// </summary>
/// <param name="msg"></param>
void ConsoleHandler::handleCheckFlag(const std::string& msg) {
    if (!isSupportedFileFormat(_filePath)) {
        printMessage(Messages::MSG_UNSUPPORTED_FILE_FROMAT);
        return;
    }

    // Check if a message can be encoded or is already encoded in the file at filePath
    if (_fileHandler->checkIfCanRead(_filePath)) {
        printMessage(Messages::MSG_UNABLE_TO_WRITE);
		std::cout << "File is already encoded with a message." << std::endl;
        return;
    }
	
    if (!_fileHandler->checkIfCanWrite(_filePath, msg)) {
        printMessage(Messages::MSG_UNABLE_TO_WRITE);
        return;
    }

	std::cout << "Message can be encoded in file" << std::endl;
}

/// <summary>
/// Handles the Help Flag and prints the help message.
/// </summary>
void ConsoleHandler::handleHelpFlag() {
    // Print information about the program, its supported flags and arguments, and how to use it
    std::cout << "-i (--info): This flag expects a file path to be specified later. The program should check if the path leads to a file with" <<
        "a supported format(e.g..png or .jpg).If the file has an unsupported format(e.g..gif or .txt), the program should display an error message." << 
        "If the file has a supported format, the program should display information about the formatand the file(e.g.image size, memory usage, " <<
            " and last modification timestamp)." << std::endl << std::endl
		
        << "-e (--encrypt): This flag expects a file path and a message to be specified later.The message should be enclosed in quotation marks to be" <<
        "treated as a single argument.The program should open the image file and save the specified message in it.As with the - i flag," <<
        "the program should handle errors if the file has an unsupported format." << std::endl << std::endl
		
        << "-d (--decrypt): This flag expects a file path to be specified later.The program should open the file and try to read a message from it." << 
        "As with the other flags, the program should handle errors if the file has an unsupported format." << std::endl << std::endl
		
        << "-c (--check): This flag expects a file path and a message to be specified later.The flag should check if the specified message can" <<
        "be saved in the file or if a message is already hidden in" << std::endl << std::endl

        << "-h (--help): This flag prints the 'manual' for this program how it should be operated and what each flag expects," << 
        "which is what you are reading right now :)" << std::endl;
}

/// <summary>
/// Private helper for printing the messages on console depening on the message type.
/// </summary>
/// <param name="msg">Enum Message that determines which message should be displayed</param>
/// <param name="arg">Optional string that could be displayed on specifi message</param>
void ConsoleHandler::printMessage(Messages msg, std::string arg) const {
    switch (msg)
    {
    case Messages::MSG_MISSING_FILEPATH:
		std::cout << "Error: No file path given." << std::endl;
		break;
    case Messages::MSG_MISSING_FILEPATH_ARGUMENT:
        std::cout << "Error: missing file path argument for " << arg << " flag" << std::endl;
        break;
    case Messages::MSG_UNKNOWN_FLAG:
        std::cout << "Error: unknown flag.\nYou could try to use flag -h or --help for information on how to use the program." << std::endl;
        break;
    case Messages::MSG_UNSUPPORTED_FILE_FROMAT:
        std::cout << "Error: unsupported file format" << std::endl;
        break;
	case Messages::MSG_UNABLE_TO_WRITE:
		std::cout << "Error: unable to write to file" << std::endl;
		break;
    case Messages::MSG_UNABLE_TO_READ:
		std::cout << "Error: unable to read from file" << std::endl;
		break;
    case Messages::MSG_UNABLE_TO_ENCODE:
		std::cout << "Error: unable to encode message" << std::endl;
		break;
    case Messages::MSG_UNABLE_TO_DECODE:
		std::cout << "Error: unable to decode message" << std::endl;
		break;
    case Messages::MSG_NOT_ENCODED:
        std::cout << "Eror: message is not encoded" << std::endl;
        break;
    case Messages::MSG_MISSING_MESSAGE_TO_ENCODE:
        std::cout << "Error: missing message argument for " << arg << " flag" << std::endl;
        break;
    default:
		std::cout << "Error: unknown message" << std::endl;
        break;
    }
}

/// <summary>
/// Main method that handles the console input - flags
/// </summary>
/// <param name="argc">Number of arguments</param>
/// <param name="argv">Arguments passed by user</param>
void ConsoleHandler::handleConsoleInput(int argc, char* argv[]) {
    if (argc <= 1) { // If users didnot not provide anything when launching call the help flag
        handleHelpFlag();
        return;
    }
	
	// Parse the command line arguments and call the appropriate handler function
    std::string arg = argv[1];
    if (argc > 2) {
        _filePath = argv[2];
    }
	
    if (arg == "-i" || arg == "--info") { // Info flag
        if (argc <= 2) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        }
        handleInfoFlag();
    }
    else if (arg == "-e" || arg == "--encode") { // Encode flag
        if (argc <= 2) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        } else if (argc <= 3) {
            printMessage(Messages::MSG_MISSING_MESSAGE_TO_ENCODE, arg);
            return;
        }
        handleEncodeFlag(argv[3]);
    }
    else if (arg == "-d" || arg == "--decode") { // Decode flag
        if (argc <= 2) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        }
        handleDecodeFlag();
    }
    else if (arg == "-c" || arg == "--check") { // Check flag
        if (argc <= 2) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        }
        else if (argc <= 3) {
            printMessage(Messages::MSG_MISSING_MESSAGE_TO_ENCODE, arg);
            return;
        }
        handleCheckFlag(argv[3]);
    }
    else if (arg == "-h" || arg == "--help") { // Help flag
        handleHelpFlag();
    }
    else { // Default case - unknown flag
        printMessage(Messages::MSG_UNKNOWN_FLAG);
    }
}