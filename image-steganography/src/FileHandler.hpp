#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "structs.hpp"
#include "enums.hpp"
#include "ImageHandler.hpp"

class FileHandler {
private:
	ImageHandler* _imageHandler;

	bool readImage(const std::string& filePath, Image& image) const;
	bool writeImage(const std::string& filePath, const Image& image) const;
	bool readPNGImage(std::ifstream& file, Image& image) const;
	bool readBMPImage(std::ifstream& file, Image& image) const;
public:
	FileHandler() {
		_imageHandler = new ImageHandler();
	}
	~FileHandler() {
		_imageHandler->~ImageHandler();
		delete _imageHandler;
	}
	
	bool checkIfCanWrite(const std::string& filePath, const std::string& msg) const;
	bool checkIfCanRead(const std::string& filePath) const;
	bool getInfoImage(const std::string& filePath, Image& image) const;
	
	bool encodeMessage(const std::string& filePath, const std::string& message) const;
	std::string decodeMessage(const std::string& filePath) const;
};