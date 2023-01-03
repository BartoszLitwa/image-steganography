#pragma once
#include "ImageHandler.hpp"

bool ImageHandler::encodeMessageInImage(Image& image, const std::string& message) const {
    // Calculate the number of pixels needed to store the message
// Each pixel can store 3 bits of the message (one in each channel), so we need at least as many pixels as the message length in bits divided by 3
    int numPixels = (message.length() * 8) / (image.bitsPerPixel / 8) + 1; // +1 to store the message length at the start
    if (numPixels > image.width * image.height)
    {
        std::cout << "Error: message is too long to fit in the image" << std::endl;
        return false;
    }

    // Encode the message length at the start of the pixel data
    image.pixels[0].red = message.length();

    // Encode the message itself in the remaining pixel data
    int pixelIndex = 1; // Start at the second pixel (index 1) to leave room for the message length
    int bitIndex = 0; // Keep track of the current bit being encoded
    for (int i = 0; i < message.length(); i++) {
        // Extract each bit of the character and store it in the corresponding pixel's color
        for (int bit = 7; bit >= 0; bit--) {
            // Use a bitmask to extract the bit at the current position
            unsigned char bitmask = 1 << bit;
            unsigned char bitValue = message[i] & bitmask;

            // Store the extracted bit in the pixel's color
            switch (bitIndex % 3) {
            case 0:
                image.pixels[pixelIndex].red = bitValue ? 1 : 0;
                break;
            case 1:
                image.pixels[pixelIndex].green = bitValue ? 1 : 0;
                break;
            case 2:
                image.pixels[pixelIndex].blue = bitValue ? 1 : 0;
                break;
            }

            bitIndex++;

            // If all three bits have been encoded, move on to the next pixel
            if (bitIndex % (image.bitsPerPixel / 8) == 0)
                pixelIndex++;
        }
    }

    // Return success
    return true;
}

std::string ImageHandler::decodeMessageInImage(const Image& image) const {
    return "";
}

bool ImageHandler::checkIfImageIsEncoded(const Image& image) const {
    // Calculate the number of pixels needed to encode the string
    int numPixels = (_messageEncoded.length() * 8) / (image.bitsPerPixel / 8) + 1; // +1 to store the message length at the start
    if (numPixels > image.width * image.height)
    {
        std::cout << "Error: message is too long to fit in the image" << std::endl;
        return false;
    }

    // Check the first few pixels of the image to see if they contain the encoded string
    int pixelIndex = 0;
    int bitIndex = 0; // Keep track of the current bit being encoded
    int bits = _messageEncoded.length() * 8;
    for (int i = 0; i < _messageEncoded.length(); i++) {
        for (int b = 7; b >= 0; b--) {
            char expected = (_messageEncoded[i] >> b) & 1; // Get the letter - needs 8 colors
            char res;
            switch (bitIndex % 3) {
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
            // Result is different than expected
            if (expected != res)
                return false;

            bitIndex++;

            // If all three bits have been encoded, move on to the next pixel
            if (bitIndex % (image.bitsPerPixel / 8) == 0)
                pixelIndex++;

            if (bitIndex == bits) // We checked every needed 
                break;
        }
    }

    return true;
}