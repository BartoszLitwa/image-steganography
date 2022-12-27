// Structure to hold the data for a single image pixel
struct Pixel {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

// Structure to hold the data for an entire image
struct Image {
	int width;
	int height;
	Pixel* pixels;
};

// Structure to hold info about the image file
struct File {
	char* name;
	int size;
};