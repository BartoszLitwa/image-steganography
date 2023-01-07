#include <iostream>

#include "src/ConsoleHandler.hpp"

/// <summary>
/// Main method of the program
/// </summary>
/// <param name="argc">Number of arguments</param>
/// <param name="argv">List of arguments passed by user</param>
/// <returns>Returns the status code when exiting the program</returns>
int main(int argc, char* argv[]) {
	ConsoleHandler* console = new ConsoleHandler();
	console->handleConsoleInput(argc, argv);
    return 0;
}