#pragma once
#include "ConsoleHandler.hpp"

bool ConsoleHandler::isSupportedFileFormat(const std::string& path) const {
    if (path.length() == 0) {
        printMessage(Messages::MSG_MISSING_FILEPATH);
		return false;
    }
	
    // Return true if the file at the given path has a supported format .png or .bmp, false otherwise
    return Helpers::endsWith(path, ".png") || Helpers::endsWith(path, ".bmp");
}

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
	std::cout << "File format: " << _image.fileType << std::endl;
	std::cout << "File size: " << (float)_image.fileSize / 1024 / 1024 << " MB (" << _image.fileSize << " B)" << std::endl;
	std::cout << "Width: " << _image.width << " Height: " << _image.height << std::endl;
	std::cout << "Pixels: " << _image.width * _image.height << std::endl;
    std::cout << "Bits per Pixel: " << _image.bitsPerPixel << std::endl;
}

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
}

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

    std::string msg;
    if (!_fileHandler->encodeMessage(_filePath, msg)) {
        printMessage(Messages::MSG_UNABLE_TO_DECODE);
        return;
    }	
}

void ConsoleHandler::handleCheckFlag(const std::string& msg) {
    if (!isSupportedFileFormat(_filePath)) {
        printMessage(Messages::MSG_UNSUPPORTED_FILE_FROMAT);
        return;
    }

    // Check if a message can be encoded or is already encoded in the file at filePath
    if (!_fileHandler->checkIfCanWrite(_filePath, msg)) {
        printMessage(Messages::MSG_UNABLE_TO_READ);
        return;
    }
}

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
        std::cout << "Error: unknown flag" << std::endl;
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
    default:
		std::cout << "Error: unknown message" << std::endl;
        break;
    }
}

void ConsoleHandler::handleConsoleInput(int argc, char* argv[]) {
    if (argc == 0) { // If users didnot not provide anything when launching call the help flag
        handleHelpFlag();
        return;
    }
	
	// Parse the command line arguments and call the appropriate handler function
    std::string arg = argv[0];
    if (argc >= 1) {
        _filePath = argv[1];
    }
		
    if (arg == "-i" || arg == "--info") {
        if (1 >= argc) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        }
        handleInfoFlag();
    }
    else if (arg == "-e" || arg == "--encode") {
        if (2 >= argc) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        }
        handleEncodeFlag(argv[2]);
    }
    else if (arg == "-d" || arg == "--decode") {
        if (1 >= argc) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        }
        handleDecodeFlag();
    }
    else if (arg == "-c" || arg == "--check") {
        if (2 >= argc) {
            printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
            return;
        }
        handleCheckFlag(argv[2]);
    }
    else if (arg == "-h" || arg == "--help") {
        handleHelpFlag();
    }
    else {
        printMessage(Messages::MSG_UNKNOWN_FLAG);
        return;
    }
}