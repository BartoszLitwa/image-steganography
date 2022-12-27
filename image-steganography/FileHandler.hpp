#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "src/structs.hpp"

class FileHandler {
private:
	const int _maxMessageLength = 1000;
	
public:
	// Set the const private member value
	FileHandler() : _maxMessageLength(1000) {}
	FileHandler(int maxMessageLength) : _maxMessageLength(maxMessageLength){}
	~FileHandler() {}
	
	bool readImage(const std::string& filePath, Image& image);
	bool writeImage(const std::string& filePath, const Image& image);
	bool hideMessage(Image& image, const std::string& message);
	std::string retrieveMessage(const Image& image);
};