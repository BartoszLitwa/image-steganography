#include "src/ConsoleHandler.hpp"

bool ConsoleHandler::isSupportedFileFormat(std::string path)
{
    if (path.length() == 0) {
		std::cout << "Error: No file path given." << std::endl;
		return false;
    }
	
    // Return true if the file at the given path has a supported format .png or .bmp, false otherwise
    return Helpers::endsWith(path, ".png") || Helpers::endsWith(path, ".bmp");
}

void ConsoleHandler::handleInfoFlag()
{
    if (!isSupportedFileFormat(filePath))
    {
        std::cout << "Error: unsupported file format" << std::endl;
        return;
    }

    // Display information about the file at filePath, such as its size, memory usage, and last modification timestamp
}

void ConsoleHandler::handleEncodeFlag()
{
    if (!isSupportedFileFormat(filePath))
    {
        std::cout << "Error: unsupported file format" << std::endl;
        return;
    }

    // Open the file at filePath and encode the message into it
}

void ConsoleHandler::handleDecodeFlag()
{
    if (!isSupportedFileFormat(filePath))
    {
        std::cout << "Error: unsupported file format" << std::endl;
        return;
    }

    // Open the file at filePath and decode any message stored in it
}

void ConsoleHandler::handleCheckFlag() {
    if (!isSupportedFileFormat(filePath)) {
        std::cout << "Error: unsupported file format" << std::endl;
        return;
    }

    // Check if a message can be encoded or is already encoded in the file at filePath
}

void ConsoleHandler::handleHelpFlag() {
    // Print information about the program, its supported flags and arguments, and how to use it
}

void ConsoleHandler::handleUnknownFlag() {
	std::cout << "Error: unknown flag" << std::endl;
}

void ConsoleHandler::handleConsoleInput(int argc, char* argv[]) {
	// Parse the command line arguments and call the appropriate handler function
	
    // No arguments were given - argc will be 1
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
		
		
        if (arg == "-i" || arg == "--info") {
            if (i + 1 >= argc) {
                std::cout << "Error: missing file path argument for -i flag" << std::endl;
                continue;
            }
            handleInfoFlag();
        }
        else if (arg == "-e" || arg == "--encode") {
            if (i + 2 >= argc) {
                std::cout << "Error: missing file path and message arguments for " << " " << " flag" << std::endl;
                continue;
            }
            handleEncodeFlag();
        }
        else if (arg == "-d" || arg == "--decode") {
            if (i + 1 >= argc) {
                std::cout << "Error: missing file path argument for -d flag" << std::endl;
                continue;
            }
            handleDecodeFlag();
        }
        else if (arg == "-c" || arg == "--check") {
            if (i + 2 >= argc) {
                std::cout << "Error: missing file path and message arguments for -c flag" << std::endl;
                continue;
            }
            handleCheckFlag();
        }
        else if (arg == "-h" || arg == "--help") {
            handleHelpFlag();
        }
        else {
            handleUnknownFlag();
        }
    }
}