#pragma once
#include "FileHandler.hpp"

// Function to read an image from a file
bool FileHandler::readImage(const std::string& filePath, Image& image) const {
	// Open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) { // We couldnt open it
		return false;
	}

	// Read the BMP file header
	uint16_t fileType;
	file.read((char*)&fileType, sizeof(fileType));
	switch (fileType) {
		case FileType::BMP: // 0x4D42 is 19778 which is BMP file format flag
			readBMPImage(file, image);
			break;
		case FileType::PNG: // 55551 - 0xD8FF - png file format flag
			readPNGImage(file, image);
			break;
		default:
			return false;
	}
	image.fileType = fileTypeToString.at(FileType::PNG);

	// Close the file and return success
	file.close();
	return true;
}

// Function to write an image to a file
bool FileHandler::writeImage(const std::string& filePath, const Image& image) const {
	// Open the file in binary mode
	std::ofstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		return false;
	}

	// Move to bytes where pixels start
	file.seekp(image.dataOffset);
	// Write the pixel data to the file
	file.write((char*)image.pixels, image.width * image.height * sizeof(Pixel));

	// Close the file and return success
	file.close();
	return true;
}

bool FileHandler::readPNGImage(std::ifstream& file, Image& image) const {
	return true;
}

bool FileHandler::readBMPImage(std::ifstream& file, Image& image) const {
	file.read((char*)&image.fileSize, sizeof(image.fileSize));

	uint32_t reserved;
	file.read((char*)&reserved, sizeof(reserved));

	file.read((char*)&image.dataOffset, sizeof(image.dataOffset));

	// Read the header size
	uint32_t headerSize;
	file.read((char*)&headerSize, sizeof(headerSize));

	// Read the image width and height
	file.read((char*)&image.width, sizeof(image.width));
	file.read((char*)&image.height, sizeof(image.height));

	uint16_t planes;
	file.read((char*)&planes, sizeof(planes));

	file.read((char*)&image.bitsPerPixel, sizeof(image.bitsPerPixel));

	uint32_t compression;
	file.read((char*)&compression, sizeof(compression));

	file.read((char*)&image.dataSize, sizeof(image.dataSize));

	image.pixels = new Pixel[image.width * image.height];

	file.seekg(image.dataOffset);
	int bytesPerPixel = image.bitsPerPixel / 8;
	for (int y = 0; y < image.height; ++y) {
		for (int x = 0; x < image.width; ++x) {
			file.read((char*)&image.pixels[y * image.width + x], bytesPerPixel);
		}
	}

	return true;
}

// Function to hide a message within an image
bool FileHandler::encodeMessage(const std::string& filePath, const std::string& message) const {
	Image image;
	if (!readImage(filePath, image)) {
		return false;
	}

	// Decode the message length from the first pixel
	if (_imageHandler->checkIfImageIsEncoded(image)) {
		return "Image is Encoded";
	}

	// Encode message in image
	bool status = _imageHandler->encodeMessageInImage(image, message);

	// Save the encoded message to image
	if (!status || !writeImage(filePath, image)) {
		return false;
	}
	
	// Return success
	return true;
}

std::string FileHandler::decodeMessage(const std::string& filePath) const {
	// Initialize an empty string to hold the retrieved message
	std::stringstream ss;

	Image image;
	if (!readImage(filePath, image)) {
		return "Cannot read image";
	}

	// Decode the message length from the first pixel
	if (!_imageHandler->checkIfImageIsEncoded(image)) {
		return "Image is Not Encoded";
	}
	
	// Return the retrieved message
	return _imageHandler->decodeMessageInImage(image);
}

bool FileHandler::checkIfCanWrite(const std::string& filePath, const std::string& msg) const {
	Image image;
	if (!readImage(filePath, image)) {
		return false;
	}

	// Return success
	return true;
}

bool FileHandler::checkIfCanRead(const std::string& filePath) const {
	// Initialize an empty string to hold the retrieved message
	Image image;
	if (!readImage(filePath, image)) { // Read the image
		return false;
	}
	
	return _imageHandler->checkIfImageIsEncoded(image);
}

bool FileHandler::getInfoImage(const std::string& filePath, Image& image) const {
	return readImage(filePath, image);
}