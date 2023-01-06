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

	/// <summary>
	/// Read the image depending on the file type and return the image data
	/// </summary>
	/// <param name="filePath">Filepath from which the image data will be read from</param>
	/// <param name="image">Image to which data will be saved</param>
	/// <returns>Returns if the image has been successfully read</returns>
	bool readImage(const std::string& filePath, Image& image) const;
	/// <summary>
	/// Save the modified pixels data with encoded message to the image
	/// </summary>
	/// <param name="filePath">Filepath to which the modfied image data will be saved</param>
	/// <param name="image">Image that holds the modfied data of the image</param>
	/// <returns>Returns if the image has been successfully saved</returns>
	bool writeImage(const std::string& filePath, const Image& image) const;
	/// <summary>
	/// Helper method for readImage that reads the image data from a .png file
	/// </summary>
	/// <param name="file">Input File Stream that has opened the requested .png file</param>
	/// <param name="image">Image to which data will be saved</param>
	/// <returns>Returns if the .png image has been successfully read</returns>
	bool readPNGImage(std::ifstream& file, Image& image) const;
	/// <summary>
	/// Helper method for readImage that reads the image data from a .bmp file
	/// </summary>
	/// <param name="file">Input File Stream that has opened the requested .bmp file</param>
	/// <param name="image">Image to which data will be saved</param>
	/// <returns>Returns if the .bmp image has been successfully read</returns>
	bool readBMPImage(std::ifstream& file, Image& image) const;
public:
	/// <summary>
	/// Constructor
	/// </summary>
	FileHandler() {
		_imageHandler = new ImageHandler();
	}
	/// <summary>
	/// Destructor
	/// </summary>
	~FileHandler() {
		_imageHandler->~ImageHandler();
		delete _imageHandler;
	}
	
	/// <summary>
	/// Determine if the image under this path has been already encoded and could hold the message
	/// File is big enough to save the message inside
	/// </summary>
	/// <param name="filePath">Filepath from which the image data will be read from</param>
	/// <param name="msg">Message that would be potentially saved to file</param>
	/// <returns>Returns true if message could be stored in this file</returns>
	bool checkIfCanWrite(const std::string& filePath, const std::string& msg) const;
	/// <summary>
	/// Checks if the file is a valid image file and if it has already a message encoded
	/// </summary>
	/// <param name="filePath">Filepath from which the image data will be read from</param>
	/// <returns>Returns true if the file holds encoded message that could be read</returns>
	bool checkIfCanRead(const std::string& filePath) const;
	/// <summary>
	/// Read the image and return the image data for the requested file
	/// </summary>
	/// <param name="filePath">Filepath from which the image data will be read from</param>
	/// <param name="image">Modifies the passed image with the images data</param>
	/// <returns>Returns true if succesffully retrieved data from the image in filepath</returns>
	bool getInfoImage(const std::string& filePath, Image& image) const;
	
	/// <summary>
	/// Encodes the message into the image and saves the modified image to the file
	/// </summary>
	/// <param name="filePath">Filepath to which the modfied image data will be saved to</param>
	/// <param name="message"></param>
	/// <returns></returns>
	bool encodeMessage(const std::string& filePath, const std::string& message) const;
	/// <summary>
	/// Retrieves the encoded message from the image under the given filepath
	/// </summary>
	/// <param name="filePath">Filepath from which the image data will be read from</param>
	/// <returns>Encoded message in the image under this path</returns>
	std::string decodeMessage(const std::string& filePath) const;
};