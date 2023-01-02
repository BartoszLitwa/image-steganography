#include <iostream>

#include "src/ConsoleHandler.hpp"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>

int main(int argc, char* argv[])
{
	argc = 3;
	std::string s1 = "--info";
	std::string s2 = "path.cpp";

	// copy the string s1 into argv[0]
	argv[0] = new char[s1.length() + 1];
	strcpy_s(argv[0], s1.length() + 1, s1.c_str());

	// copy the string s2 into argv[1]
	argv[1] = new char[s2.length() + 1];
	strcpy_s(argv[1], s2.length() + 1, s2.c_str());

	// set argv[2] to null
	argv[2] = NULL;
	
    ConsoleHandler* console = new ConsoleHandler();
	console->handleConsoleInput(argc, argv);
    return 0;
}