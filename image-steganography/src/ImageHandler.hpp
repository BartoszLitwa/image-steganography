#pragma once
#include <string>
#include <iostream>
#include <sstream>

#include "structs.hpp"

class ImageHandler {
private:
	const std::string _messageEncoded = "msgEncoded";
	const int _pixelsNeededToAllocateLength = 16; // 16 pixels - 48 bit - 6 chars
	bool encodeMessage(Image& image, const std::string& message, const int& startPixel = 0) const;
	std::string decodeMessage(const Image& image, const int& startPixel = 0, const int& pixelsAlocated = 0) const;
	int getPixelsNeededToAlocate(const std::string& message, const int& bitsPerPixel = 24) const;
	uint8_t replaceLastBit(uint8_t& val, unsigned char bit) const;

public:
	ImageHandler() {}
	~ImageHandler() {}

	bool encodeMessageInImage(Image& image, const std::string& message) const;
	std::string decodeMessageInImage(const Image& image) const;
	bool checkIfImageIsEncoded(const Image& image) const;
};
