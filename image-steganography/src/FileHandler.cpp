#pragma once
#include "FileHandler.hpp"

/// <summary>
/// Read the image depending on the file type and return the image data
/// </summary>
/// <param name="filePath">Filepath from which the image data will be read from</param>
/// <param name="image">Image to which data will be saved</param>
/// <returns>Returns if the image has been successfully read</returns>
bool FileHandler::readImage(const std::string& filePath, Image& image) const {
	// Open the file in binary mode
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open()) { // We couldnt open it
		return false;
	}
	
	bool status = true;
	if(Helpers::endsWith(filePath, ".bmp")) {
		status = readBMPImage(file, image);
	}
	else if(Helpers::endsWith(filePath, ".ppm")) {
		status = readPPMImage(file, image);
	}

	// Close the file and return success
	file.close();
	return status;
}

/// <summary>
/// Save the modified pixels data with encoded message to the image
/// </summary>
/// <param name="filePath">Filepath to which the modfied image data will be saved</param>
/// <param name="image">Image that holds the modfied data of the image</param>
/// <returns>Returns if the image has been successfully saved</returns>
bool FileHandler::writeImage(const std::string& filePath, const Image& image) const {
	// Open the file in binary mode
	std::ofstream file(filePath, std::ios::binary);
	if (!file.is_open()) {
		return false;
	}

	bool status = true;
	if (Helpers::endsWith(filePath, ".bmp")) {
		status = writeBMPImage(file, image);
	}
	else if (Helpers::endsWith(filePath, ".ppm")) {
		status = writePPMImage(file, image);
	}

	// Close the file and return success
	file.close();
	return status;
}

/// <summary>
/// Helper method for writeImage that saves the modfied image data to a .ppm file
/// </summary>
/// <param name="file">Input File Stream that has opened the requested .ppm file</param>
/// <param name="image">Image from which data will be read from</param>
/// <returns>Returns if the .ppm image has been successfully saved</returns>
bool FileHandler::writePPMImage(std::ofstream& file, const Image& image) const {
	file << image.ppm.magicNumber << std::endl;
	if (image.ppm.comments != "") {
		file << image.ppm.comments;
	}
	file << image.width << " " << image.height << std::endl;
	if(image.ppm.max_value != 0)
		file << image.ppm.max_value << std::endl;
	
	// Write the pixel data to the file
	int bytesPerPixel = image.bitsPerPixel / 8;
	for (int y = 0; y < image.height; ++y) {
		for (int x = 0; x < image.width; ++x) {
			file.write((char*)&image.pixels[y * image.width + x], bytesPerPixel);
			file << "	";
			/*file.write((char*)&image.pixels[y * image.width + x], bytesPerPixel);
			Pixel pi = image.pixels[y * image.width + x];
			int a = 1;*/
		}
	}

	// Close the file and return success
	file.close();
	return true;
}

/// <summary>
/// Helper method for writeImage that saves the modfied image data to a .bmp file
/// </summary>
/// <param name="file">Input File Stream that has opened the requested .bmp file</param>
/// <param name="image">Image from which data will be read from</param>
/// <returns>Returns if the .bmp image has been successfully saved</returns>
bool FileHandler::writeBMPImage(std::ofstream& file, const Image& image) const {
	// Seek to the pixel data
	file.write((char*)&image.fileType, sizeof(image.fileType));

	file.write((char*)&image.fileSize, sizeof(image.fileSize));

	file.write((char*)&image.bmp.reserved, sizeof(image.bmp.reserved));

	file.write((char*)&image.dataOffset, sizeof(image.dataOffset));

	// write the header size
	file.write((char*)&image.bmp.infoHeaderSize, sizeof(image.bmp.infoHeaderSize));

	// write the image width and height
	file.write((char*)&image.width, sizeof(image.width));
	file.write((char*)&image.height, sizeof(image.height));

	file.write((char*)&image.bmp.planes, sizeof(image.bmp.planes));
	// write the bits per pixel
	file.write((char*)&image.bitsPerPixel, sizeof(image.bitsPerPixel));

	file.write((char*)&image.bmp.compression, sizeof(image.bmp.compression));
	file.write((char*)&image.dataSize, sizeof(image.dataSize));
	file.write((char*)&image.bmp.xPixelsPerMeter, sizeof(image.bmp.xPixelsPerMeter));
	file.write((char*)&image.bmp.yPixelsPerMeter, sizeof(image.bmp.yPixelsPerMeter));
	file.write((char*)&image.bmp.colorsInColorTable, sizeof(image.bmp.colorsInColorTable));
	file.write((char*)&image.bmp.importantColorCount, sizeof(image.bmp.importantColorCount));
	
	file.seekp(image.dataOffset);
	// Write the pixel data to the file
	const int paddingAmount = (4 - (image.width * (image.bitsPerPixel / 8)) % 4) % 4;
	unsigned char bmpPad[3] = {0, 0, 0};
	
	int bytesPerPixel = image.bitsPerPixel / 8;
	for (int y = 0; y < image.height; ++y) {
		for (int x = 0; x < image.width; ++x) {
			file.write((char*)&image.pixels[y * image.width + x], bytesPerPixel);
		}
		file.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}

	// Close the file and return success
	return true;
}

/// <summary>
/// Helper method for readImage that reads the image data from a .ppm file
/// </summary>
/// <param name="file">Input File Stream that has opened the requested .ppm file</param>
/// <param name="image">Image to which data will be saved</param>
/// <returns>Returns if the .ppm image has been successfully read</returns>
bool FileHandler::readPPMImage(std::ifstream& file, Image& image) const {
	// Read the BMP file header
	image.fileType = FileType::PPM;
	image.bitsPerPixel = 24;
	
	PPMImage ppm;
	std::getline(file, ppm.magicNumber);
	if (ppm.magicNumber != "P6" && ppm.magicNumber != "P3") {
		std::cout << "Error: Invalid PPM format" << std::endl;
		return false;
	}

	// Read the next three lines and extract the image width, height, and maximum value
	std::string line;
	for (int i = 0; i < 3; ++i) {
		std::getline(file, line);
		std::stringstream ss(line);
		if (line[0] == '#') {
			ppm.comments += line + "\n";
			// Skip commented lines
			--i;
			continue;
		}
		if (i == 0) {
			// Extract the width and height from the same line
			ss >> image.width >> image.height;
			ppm.width = image.width;
			ppm.height = image.height;
		}
		else if (i == 1) {
			// Extract the maximum value
			ss >> ppm.max_value;
		}
	}
	
	image.ppm = ppm;

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

/// <summary>
/// Helper method for readImage that reads the image data from a .bmp file
/// </summary>
/// <param name="file">Input File Stream that has opened the requested .bmp file</param>
/// <param name="image">Image to which data will be saved</param>
/// <returns>Returns if the .bmp image has been successfully read</returns>
bool FileHandler::readBMPImage(std::ifstream& file, Image& image) const {
	BMPImage bmpImage;
	// Read the BMP file header
	file.read((char*)&image.fileType, sizeof(image.fileType)); 
	
	file.read((char*)&image.fileSize, sizeof(image.fileSize));

	file.read((char*)&bmpImage.reserved, sizeof(bmpImage.reserved));

	file.read((char*)&image.dataOffset, sizeof(image.dataOffset));

	// Read the header size
	file.read((char*)&bmpImage.infoHeaderSize, sizeof(bmpImage.infoHeaderSize));

	// Read the image width and height
	file.read((char*)&image.width, sizeof(image.width));
	file.read((char*)&image.height, sizeof(image.height));

	file.read((char*)&bmpImage.planes, sizeof(bmpImage.planes));
	// Read the bits per pixel
	file.read((char*)&image.bitsPerPixel, sizeof(image.bitsPerPixel));

	file.read((char*)&bmpImage.compression, sizeof(bmpImage.compression));
	file.read((char*)&image.dataSize, sizeof(image.dataSize));
	file.read((char*)&bmpImage.xPixelsPerMeter, sizeof(bmpImage.xPixelsPerMeter));
	file.read((char*)&bmpImage.yPixelsPerMeter, sizeof(bmpImage.yPixelsPerMeter));
	file.read((char*)&bmpImage.colorsInColorTable, sizeof(bmpImage.colorsInColorTable));
	file.read((char*)&bmpImage.importantColorCount, sizeof(bmpImage.importantColorCount));

	image.pixels = new Pixel[image.width * image.height];
	image.bmp = bmpImage;
	
	const int paddingAmount = (4 - (image.width * (image.bitsPerPixel / 8)) % 4) % 4;

	file.seekg(image.dataOffset);
	int bytesPerPixel = image.bitsPerPixel / 8;
	for (int y = 0; y < image.height; ++y) {
		for (int x = 0; x < image.width; ++x) {
			file.read((char*)&image.pixels[y * image.width + x], bytesPerPixel);
		}
		// Account for each padding after each row
		file.ignore(paddingAmount);
	}

	return true;
}

/// <summary>
/// Encodes the message into the image and saves the modified image to the file
/// </summary>
/// <param name="filePath">Filepath to which the modfied image data will be saved to</param>
/// <param name="message"></param>
/// <returns></returns>
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

/// <summary>
/// Retrieves the encoded message from the image under the given filepath
/// </summary>
/// <param name="filePath">Filepath from which the image data will be read from</param>
/// <returns>Encoded message in the image under this path</returns>
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

/// <summary>
/// Determine if the image under this path has been already encoded and could hold the message
/// File is big enough to save the message inside
/// </summary>
/// <param name="filePath">Filepath from which the image data will be read from</param>
/// <param name="msg">Message that would be potentially saved to file</param>
/// <returns>Returns true if message could be stored in this file</returns>
bool FileHandler::checkIfCanWrite(const std::string& filePath, const std::string& msg) const {
	Image image;
	if (!readImage(filePath, image)) {
		return false;
	}

	// Return success
	return true;
}

/// <summary>
/// Checks if the file is a valid image file and if it has already a message encoded
/// </summary>
/// <param name="filePath">Filepath from which the image data will be read from</param>
/// <returns>Returns true if the file holds encoded message that could be read</returns>
bool FileHandler::checkIfCanRead(const std::string& filePath) const {
	Image image;
	if (!readImage(filePath, image)) { // Read the image
		return false;
	}
	
	return _imageHandler->checkIfImageIsEncoded(image);
}

/// <summary>
/// Read the image and return the image data for the requested file
/// </summary>
/// <param name="filePath">Filepath from which the image data will be read from</param>
/// <param name="image">Modifies the passed image with the images data</param>
/// <returns>Returns true if succesffully retrieved data from the image in filepath</returns>
bool FileHandler::getInfoImage(const std::string& filePath, Image& image) const {
	return readImage(filePath, image);
}