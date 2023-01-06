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
	file.read((char*)&image.fileType, sizeof(image.fileType));
	switch (image.fileType) {
		case FileType::BMP: // 0x4D42 is 19778 which is BMP file format flag
			readBMPImage(file, image);
			break;
		case FileType::PNG: // 55551 - 0xD8FF - png file format flag
			readPNGImage(file, image);
			break;
		default:
			readBMPImage(file, image);
	}

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

	// Write the file header
	file.write((char*)&image.fileType, sizeof(image.fileType));
	file.write((char*)&image.fileSize, sizeof(image.fileSize));
	file.write((char*)&image.reserved, sizeof(image.reserved));
	file.write((char*)&image.dataOffset, sizeof(image.dataOffset));

	// Write the info header
	file.write((char*)&image.infoHeaderSize, sizeof(image.infoHeaderSize));
	file.write((char*)&image.width, sizeof(image.width));
	file.write((char*)&image.height, sizeof(image.height));
	file.write((char*)&image.planes, sizeof(image.planes));
	file.write((char*)&image.bitsPerPixel, sizeof(image.bitsPerPixel));
	
	file.write((char*)&image.compression, sizeof(image.compression));
	file.write((char*)&image.dataSize, sizeof(image.dataSize));
	
	file.write((char*)&image.xPixelsPerMeter, sizeof(image.xPixelsPerMeter));
	file.write((char*)&image.yPixelsPerMeter, sizeof(image.yPixelsPerMeter));
	file.write((char*)&image.colorsInColorTable, sizeof(image.colorsInColorTable));
	file.write((char*)&image.importantColorCount, sizeof(image.importantColorCount));

	// Seek to the pixel data
	file.seekp(image.dataOffset);
	// Write the pixel data to the file
	const int paddingAmount = ((4 - (image.width * 3) % 4) % 4);
	unsigned char bmpPad[2] = {0, 0};
	
	int bytesPerPixel = image.bitsPerPixel / 8;
	for (int y = 0; y < image.height; ++y) {
		for (int x = 0; x < image.width; ++x) {
			file.write((char*)&image.pixels[y * image.width + x], bytesPerPixel);
		}
		file.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}

	// Close the file and return success
	file.close();
	return true;
}

bool FileHandler::readPNGImage(std::ifstream& file, Image& image) const {
	// Read the file size
	file.read((char*)&image.fileSize, sizeof(image.fileSize));

	// Read the reserved value
	file.read((char*)&image.reserved, sizeof(image.reserved));

	// Read the data offset
	file.read((char*)&image.dataOffset, sizeof(image.dataOffset));

	// Read the header size
	file.read((char*)&image.infoHeaderSize, sizeof(image.infoHeaderSize));

	// Read the image width and height
	file.read((char*)&image.width, sizeof(image.width));
	file.read((char*)&image.height, sizeof(image.height));

	// Read the planes
	file.read((char*)&image.planes, sizeof(image.planes));

	// Read the bits per pixel
	file.read((char*)&image.bitsPerPixel, sizeof(image.bitsPerPixel));

	// Read the compression method
	file.read((char*)&image.compression, sizeof(image.compression));

	// Read the image data size
	file.read((char*)&image.dataSize, sizeof(image.dataSize));

	// Seek to the pixel data offset
	file.seekg(image.dataOffset);

	// Read the pixel data
	// Allocate memory for the pixels
	image.pixels = new Pixel[image.width * image.height];
	int bytesPerPixel = image.bitsPerPixel / 8;
	for (int y = 0; y < image.height; ++y) {
		for (int x = 0; x < image.width; ++x) {
			file.read((char*)&image.pixels[y * image.width + x], bytesPerPixel);
		}
	}

	return true;
}

bool FileHandler::readBMPImage(std::ifstream& file, Image& image) const {
	file.read((char*)&image.fileSize, sizeof(image.fileSize));

	file.read((char*)&image.reserved, sizeof(image.reserved));

	file.read((char*)&image.dataOffset, sizeof(image.dataOffset));

	// Read the header size
	file.read((char*)&image.infoHeaderSize, sizeof(image.infoHeaderSize));

	// Read the image width and height
	file.read((char*)&image.width, sizeof(image.width));
	file.read((char*)&image.height, sizeof(image.height));

	file.read((char*)&image.planes, sizeof(image.planes));

	file.read((char*)&image.bitsPerPixel, sizeof(image.bitsPerPixel));

	file.read((char*)&image.compression, sizeof(image.compression));
	file.read((char*)&image.dataSize, sizeof(image.dataSize));
	
	file.read((char*)&image.xPixelsPerMeter, sizeof(image.xPixelsPerMeter));
	file.read((char*)&image.yPixelsPerMeter, sizeof(image.yPixelsPerMeter));
	file.read((char*)&image.colorsInColorTable, sizeof(image.colorsInColorTable));
	file.read((char*)&image.importantColorCount, sizeof(image.importantColorCount));

	image.pixels = new Pixel[image.width * image.height];
	
	const int paddingAmount = ((4 - (image.width * 3) % 4) % 4);

	file.seekg(image.dataOffset);
	int bytesPerPixel = image.bitsPerPixel / 8;
	for (int y = 0; y < image.height; ++y) {
		for (int x = 0; x < image.width; ++x) {
			file.read((char*)&image.pixels[y * image.width + x], bytesPerPixel);
		}
		// Account for each 
		file.ignore(paddingAmount);
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
		return false;
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
	Image image;
	if (!readImage(filePath, image)) { // Read the image
		return false;
	}
	
	return _imageHandler->checkIfImageIsEncoded(image);
}

bool FileHandler::getInfoImage(const std::string& filePath, Image& image) const {
	return readImage(filePath, image);
}