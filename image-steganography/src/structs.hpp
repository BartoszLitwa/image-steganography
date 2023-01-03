#pragma once
// Structure to hold the data for a single image pixel
struct Pixel {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

// Structure to hold the data for an entire image
struct Image {
	int32_t width;
	int32_t height;
	Pixel* pixels;
	uint32_t fileSize;
	uint16_t bitsPerPixel;
	uint32_t dataSize;
	uint32_t dataOffset;
	std::string fileType;
};