#include <iostream>

#include "src/ConsoleHandler.hpp"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>

int main(int argc, char* argv[])
{
	argc = 3;
	std::string s1 = "-h";
	std::string s2 = "E:/Development/Projects/image-steganography/image-steganography/images/2.bmp";
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