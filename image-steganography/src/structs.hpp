#pragma once
#include <string>

struct BMPImage {
	uint16_t fileType;
	uint32_t fileSize;
	uint32_t reserved;
	uint32_t dataOffset;
	uint32_t infoHeaderSize;
	uint32_t width;
	uint32_t height;
	uint16_t planes;
	uint16_t bitsPerPixel;
	uint32_t compression;
	uint32_t dataSize;
	uint32_t xPixelsPerMeter;
	uint32_t yPixelsPerMeter;
	uint32_t colorsInColorTable;
	uint32_t importantColorCount;
};

struct PPMImage {
	std::string magicNumber;
	uint32_t width;
	uint32_t height;
};

// Structure to hold the data for a single image pixel
struct Pixel {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

// Structure to hold the data for an entire image
struct Image {
	// stored as 1D array
	Pixel* pixels;
	
	uint16_t fileType;
	uint32_t fileSize;
	uint32_t dataOffset;
	uint32_t width;
	uint32_t height;
	uint16_t bitsPerPixel;
	uint32_t dataSize;

	BMPImage bmp;
	PPMImage ppm;
};