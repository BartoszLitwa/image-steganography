#include "ConsoleHandler.hpp"

bool ConsoleHandler::isSupportedFileFormat(std::string path) {
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

    // Display information about the file at filePath, such as its size, memory usage, and last modification timestamp
    if (!_fileHandler->checkIfCanRead(_filePath)) {
        printMessage(Messages::MSG_UNABLE_TO_READ);
        return;
    }
	
    _fileHandler->getInfoImage(_filePath, _image);
}

void ConsoleHandler::handleEncodeFlag(std::string msg) {
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

void ConsoleHandler::handleCheckFlag(std::string msg) {
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
    std::cout << "Program " << std::endl;
}

void ConsoleHandler::printMessage(Messages msg, std::string arg) {
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
    for (int i = 0; i < argc; i++) {
        std::string arg = argv[i];
        if (argc >= i + 1) {
            _filePath = argv[i + 1];
        }
		
        if (arg == "-i" || arg == "--info") {
            if (i + 1 >= argc) {
                printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
                continue;
            }
            handleInfoFlag();
        }
        else if (arg == "-e" || arg == "--encode") {
            if (i + 2 >= argc) {
                printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
                continue;
            }
            handleEncodeFlag(argv[i + 2]);
        }
        else if (arg == "-d" || arg == "--decode") {
            if (i + 1 >= argc) {
                printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
                continue;
            }
            handleDecodeFlag();
        }
        else if (arg == "-c" || arg == "--check") {
            if (i + 2 >= argc) {
                printMessage(Messages::MSG_MISSING_FILEPATH_ARGUMENT, arg);
                continue;
            }
            handleCheckFlag(argv[i + 2]);
        }
        else if (arg == "-h" || arg == "--help") {
            handleHelpFlag();
        }
        else {
            printMessage(Messages::MSG_UNKNOWN_FLAG);
            return;
        }
    }
}