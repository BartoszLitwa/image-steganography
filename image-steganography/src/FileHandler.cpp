#include "FileHandler.hpp"

// Function to read an image from a file
bool FileHandler::readImage(const std::string& filePath, Image& image) {
	// Open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) { // We couldnt open it
		return false;
	}

	// Read the width and height of the image
	file.read((char*)&image.width, sizeof(int));
	file.read((char*)&image.height, sizeof(int));

	// Allocate memory for the pixel data
	image.pixels = new Pixel[image.width * image.height];

	// Read the pixel data from the file
	file.read((char*)image.pixels, image.width * image.height * sizeof(Pixel));

	// Close the file and return success
	file.close();
	return true;
}

// Function to write an image to a file
bool FileHandler::writeImage(const std::string& filePath, const Image& image) {
	// Open the file in binary mode
	std::ofstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		return false;
	}

	// Write the width and height of the image
	file.write((char*)&image.width, sizeof(int));
	file.write((char*)&image.height, sizeof(int));

	// Write the pixel data to the file
	file.write((char*)image.pixels, image.width * image.height * sizeof(Pixel));

	// Close the file and return success
	file.close();
	return true;
}

// Function to hide a message within an image
bool FileHandler::hideMessage(Image& image, const std::string& message) {
	// Check that the message is not too long to fit in the image
	if (message.length() > _maxMessageLength) {
		return false;
	}

	// Encode the message length at the start of the pixel data
	image.pixels[0].red = message.length();

	// Encode the message itself in the remaining pixel data
	for (int i = 1; i < message.length(); i++) {
		image.pixels[i].red = message[i - 1];
	}

	// Return success
	return true;
}

std::string FileHandler::retrieveMessage(const Image& image) {
	// Initialize an empty string to hold the retrieved message
	std::stringstream ss;

	// Decode the message length from the first pixel
	int messageLength = image.pixels[0].red;

	// Check that the message length is within the valid range
	if (messageLength > _maxMessageLength || messageLength < 0) {
		return "";
	}

	// Decode the message itself from the remaining pixel data
	for (int i = 0; i < messageLength; i++) {
		ss << image.pixels[i + 1].red;
	}

	// Return the retrieved message
	return ss.str();
}