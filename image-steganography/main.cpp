#include <iostream>

#include "src/ConsoleHandler.hpp"
#include <string.h>

/// <summary>
/// Main method of the program
/// </summary>
/// <param name="argc">Number of arguments</param>
/// <param name="argv">List of arguments passed by user</param>
/// <returns>Returns the status code when exiting the program</returns>
int main(int argc, char* argv[])
{
	argc = 3;
	std::string s1 = "-e";
	std::string s2 = "E:/Development/Projects/image-steganography/image-steganography/images/1.ppm";
	std::string s3 = "message to be encoded";

	// copy the string s1 into argv[0]
	argv[0] = new char[s1.length() + 1];
	strcpy_s(argv[0], s1.length() + 1, s1.c_str());

	// copy the string s2 into argv[1]
	argv[1] = new char[s2.length() + 1];
	strcpy_s(argv[1], s2.length() + 1, s2.c_str());

	// set argv[2] to null
	argv[2] = new char[s3.length() + 1];
	strcpy_s(argv[2], s3.length() + 1, s3.c_str());
	
    ConsoleHandler* console = new ConsoleHandler();
	console->handleConsoleInput(argc, argv);
    return 0;
}