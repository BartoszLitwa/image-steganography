#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "structs.hpp"
#include "enums.hpp"

class FileHandler {
private:
	const std::string _messageEncoded = "msgEncoded";

	bool readImage(const std::string& filePath, Image& image);
	bool writeImage(const std::string& filePath, const Image& image);
public:
	// Set the const private member value
	FileHandler() {}
	~FileHandler() {}
	
	bool checkIfCanWrite(const std::string& filePath, const std::string& msg);
	bool checkIfCanRead(const std::string& filePath);
	bool getInfoImage(const std::string& filePath, Image& image);
	
	bool encodeMessage(const std::string& filePath, const std::string& message);
	std::string decodeMessage(const std::string& filePath);
};