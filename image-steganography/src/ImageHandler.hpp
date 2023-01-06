#pragma once
#include <string>
#include <iostream>
#include <sstream>

#include "structs.hpp"

class ImageHandler {
private:
	const std::string _messageEncoded = "msgEncoded";
	const int _pixelsNeededToAllocateLength = 16; // 16 pixels - 48 bit - 6 chars
	/// <summary>
	/// Business Logic that encoded the message in Image's pixel in LSB
	/// </summary>
	/// <param name="image">Pass the image that holds the message</param>
	/// <param name="message">Message that is going to be saved in image</param>
	/// <param name="startPixel">From which pixel we should start encoding message</param>
	/// <returns></returns>
	bool encodeMessage(Image& image, const std::string& message, const int& startPixel = 0) const;
	/// <summary>
	/// Business logic of reading the decoded message from image's pixels LSB
	/// </summary>
	/// <param name="image">Pass the image that holds the message</param>
	/// <param name="startPixel">From which pixel we should start reading the encoded message</param>
	/// <param name="pixelsAlocated">How many pixels we should be reading to get correct message</param>
	/// <returns></returns>
	std::string decodeMessage(const Image& image, const int& startPixel = 0, const int& pixelsAlocated = 0) const;
	/// <summary>
	/// Determine how many pixels are needed to store the message
	/// </summary>
	/// <param name="message">Message that is going to be stored</param>
	/// <param name="bitsPerPixel">How many bits 1 pixel stores</param>
	/// <returns></returns>
	int getPixelsNeededToAlocate(const std::string& message, const int& bitsPerPixel = 24) const;
	/// <summary>
	/// Replace the last bit of the given byte with the given bit
	/// </summary>
	/// <param name="val">Value that is going to be affected</param>
	/// <param name="bit">Bit to we want to replace the last bit of val</param>
	/// <returns></returns>
	uint8_t replaceLastBit(uint8_t& val, unsigned char bit) const;

public:
	ImageHandler() {}
	~ImageHandler() {}
	/// <summary>
	/// Encode that the message is stored in the image - at the beginig store constant message
	/// Encode the length of the message
	/// Encode the message itself
	/// </summary>
	/// <param name="image">Pass the image that holds the data of pixels</param>
	/// <param name="message">Message that will be encoded in image</param>
	/// <returns></returns>
	bool encodeMessageInImage(Image& image, const std::string& message) const;
	/// <summary>
	/// Decode the message from the image
	/// First check if the image contains the message
	/// Then decode the length of the message
	/// Then decode the message itself
	/// </summary>
	/// <param name="image">Pass the image that holds the message</param>
	/// <returns>Return Decoded Message</returns>
	std::string decodeMessageInImage(const Image& image) const;
	/// <summary>
	/// Check if image has been encoded before - stores constant message at the begining
	/// </summary>
	/// <param name="image">Pass the image that holds the message</param>
	/// <returns>Returns boolean - is the image encoded</returns>
	bool checkIfImageIsEncoded(const Image& image) const;
};
