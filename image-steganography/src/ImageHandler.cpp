#pragma once
#include "ImageHandler.hpp"

/// <summary>
/// Business Logic that encoded the message in Image's pixel in LSB
/// </summary>
/// <param name="image">Pass the image that holds the message</param>
/// <param name="message">Message that is going to be saved in image</param>
/// <param name="startPixel">From which pixel we should start encoding message</param>
/// <returns>Returns true if successfully encoded message in the image's pixels data</returns>
bool ImageHandler::encodeMessage(Image& image, const std::string& message, const int& startPixel) const
{
    std::stringstream ss;
    // Encode the message itself in the remaining pixel data
    int pixelIndex = startPixel; 
    int bitIndex = 0; // Keep track of the current bit being encoded
    for (int i = 0; i < message.length(); i++) {
        unsigned char letter = 0b00000000; // initialize to 0 binary
        // Extract each bit of the character and store it in the corresponding pixel's color
        for (int bit = 7; bit >= 0; bit--) {
            // Shift to left and get the first bit
            unsigned char bitValue = (message[i] >> bit) & 1;

            // Store the extracted bit in the pixel's color
            switch (bitIndex % (image.bitsPerPixel / 8)) {
            case 0:
                replaceLastBit(image.pixels[pixelIndex].red, bitValue);
                break;
            case 1:
                replaceLastBit(image.pixels[pixelIndex].green, bitValue);
                break;
            case 2:
                replaceLastBit(image.pixels[pixelIndex].blue, bitValue);
                break;
            }
            // If all three bits have been encoded, move on to the next pixel
            if (++bitIndex % (image.bitsPerPixel / 8) == 0)
                pixelIndex++;
		    
			letter = letter | (bitValue << bit);
        }
        ss << letter;
    }
	
    return ss.str() == message;
}

/// <summary>
/// Business logic of reading the decoded message from image's pixels LSB
/// </summary>
/// <param name="image">Pass the image that holds the message</param>
/// <param name="startPixel">From which pixel we should start reading the encoded message</param>
/// <param name="pixelsAlocated">How many pixels we should be reading to get correct message</param>
/// <returns>Returns decoded message from the modified image's pixels data</returns>
std::string ImageHandler::decodeMessage(const Image& image, const int& startPixel, const int& pixelsAlocated) const
{
    std::stringstream ss;
    // Check the first few pixels of the image to see if they contain the encoded string
    int pixelIndex = startPixel;
    int bitIndex = 0; // Keep track of the current bit being encoded
    int bits = pixelsAlocated * (image.bitsPerPixel / 8);
    for (int i = 0; i < (bits / 8); i++) {
        unsigned char letter = 0b00000000; // initialize to 0 binary
        for (int b = 7; b >= 0; b--) {
            unsigned char res;
            switch (bitIndex % 3) { // take last bit
            case 0:
                res = image.pixels[pixelIndex].red & 1;
                break;
            case 1:
                res = image.pixels[pixelIndex].green & 1;
                break;
            case 2:
                res = image.pixels[pixelIndex].blue & 1;
                break;
            }

            // Using bitwise OR operator
			letter = letter | (res << b); // shift bit to the left and add it to the letter

            // If all three bits have been encoded, move on to the next pixel
            if (++bitIndex % (image.bitsPerPixel / 8) == 0)
                pixelIndex++;

            if (bitIndex == bits) { // We checked every needed
                break;
            }
        }
        ss << letter;
    }
    return ss.str();
}

/// <summary>
/// Determine how many pixels are needed to store the message
/// </summary>
/// <param name="message">Message that is going to be stored</param>
/// <param name="bitsPerPixel">How many bits 1 pixel stores</param>
/// <returns>Returns number of pixels needed to store message</returns>
int ImageHandler::getPixelsNeededToAlocate(const std::string& message, const int& bitsPerPixel) const
{
    return (message.length() * 8) / (bitsPerPixel / 8) + 1; // +1 to store the message length at the start
}

/// <summary>
/// Replace the last bit of the given byte with the given bit
/// </summary>
/// <param name="val">Value that is going to be affected</param>
/// <param name="bit">Bit to we want to replace the last bit of val</param>
/// <returns>Returns 8 bit val with changed last bit to requeste
uint8_t ImageHandler::replaceLastBit(uint8_t& val, unsigned char bit) const
{
    // 0 will be represented by 000000 but ~1 will be 0
    val = bit == 0 ? val & ~1 : val | 1; 
    return val;
}

/// <summary>
/// Encode that the message is stored in the image - at the beginig store constant message
/// Encode the length of the message
/// Encode the message itself
/// </summary>
/// <param name="image">Pass the image that holds the data of pixels</param>
/// <param name="message">Message that will be encoded in image</param>
/// <returns>Return true if successfulyy encoded message in image</returns>
bool ImageHandler::encodeMessageInImage(Image& image, const std::string& message) const {
    // Calculate the number of pixels needed to store the message
    // Each pixel can store(usually if bits per pixel = 24) 3 bits of the message (one in each channel),
    // so we need at least as many pixels as the message length in bits divided by 3
    int numPixels = getPixelsNeededToAlocate(_messageEncoded, image.bitsPerPixel)
        + getPixelsNeededToAlocate(message, image.bitsPerPixel) + _pixelsNeededToAllocateLength;
    if (numPixels > image.width * image.height)
    {
        std::cout << "Error: message is too long to fit in the image" << std::endl;
        return false;
    }

    // Encode that the message is encoded at the begining
    if (!encodeMessage(image, _messageEncoded, 0)) {
        return false;
    }
    // Encode the number of chars that the message has
    int currentPixel = getPixelsNeededToAlocate(_messageEncoded, image.bitsPerPixel);
    std::string length = std::to_string(message.length());
    length = ((std::string)"000000").substr(0, 6 - length.length()) + length;
    if (!encodeMessage(image, length, currentPixel)) {
        return false;
    }
    currentPixel += _pixelsNeededToAllocateLength;

	// Encode the message itself in the remaining pixel data
    if (!encodeMessage(image, message, currentPixel)) {
        return false;
    }
    currentPixel += getPixelsNeededToAlocate(message, image.bitsPerPixel);

    // Return success
    return true;
}

/// <summary>
/// Decode the message from the image
/// First check if the image contains the message
/// Then decode the length of the message
/// Then decode the message itself
/// </summary>
/// <param name="image">Pass the image that holds the message</param>
/// <returns>Return Decoded Message</returns>
std::string ImageHandler::decodeMessageInImage(const Image& image) const {
    // Calculate the minimum number of pixels needed to store the message
    int numPixels = getPixelsNeededToAlocate(_messageEncoded, image.bitsPerPixel) + _pixelsNeededToAllocateLength;
    if (numPixels > image.width * image.height)
    {
        std::cout << "Error: message is too long to fit in the image" << std::endl;
        return "";
    }

    // Encode that the message is encoded at the begining
    int currentPixel = getPixelsNeededToAlocate(_messageEncoded, image.bitsPerPixel);
    std::string tempString = decodeMessage(image, 0, currentPixel);
    if (tempString.empty() || tempString != _messageEncoded) {
        return "";
    }
    // Encode the number of chars that the message has
    std::string length = decodeMessage(image, currentPixel, _pixelsNeededToAllocateLength);
    if (length.empty()) {
        return "";
    }
    currentPixel += _pixelsNeededToAllocateLength;

    // Encode the message itself in the remaining pixel data
    int pixelsMessage = (std::atoi(length.c_str()) * 8) / (image.bitsPerPixel / 8) + 1;
    std::string messageEncoded = decodeMessage(image, currentPixel, pixelsMessage);
    if (tempString.empty()) {
        return "";
    }

    // Return success
    return messageEncoded;
}

/// <summary>
/// Check if image has been encoded before - stores constant message at the begining
/// </summary>
/// <param name="image">Pass the image that holds the message</param>
/// <returns>Returns boolean - is the image encoded</returns>
bool ImageHandler::checkIfImageIsEncoded(const Image& image) const {
    // Calculate the number of pixels needed to encode the string
    int numPixels = getPixelsNeededToAlocate(_messageEncoded, image.bitsPerPixel) + _pixelsNeededToAllocateLength;
    if (numPixels > image.width * image.height)
    {
        std::cout << "Error: message is too long to fit in the image" << std::endl;
        return false;
    }

	std::string message = decodeMessage(image, 0, numPixels - _pixelsNeededToAllocateLength);
    if (message.empty() || message != _messageEncoded) { // Check if we got the message
		return false;
    }

    return true;
}