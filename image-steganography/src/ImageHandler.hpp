#pragma once
#include <string>
#include <iostream>

#include "structs.hpp"

class ImageHandler {
private:
	const std::string _messageEncoded = "msgEncoded";

public:
	ImageHandler() {}
	~ImageHandler() {}

	bool encodeMessageInImage(Image& image, const std::string& message) const;
	std::string decodeMessageInImage(const Image& image) const;
	bool checkIfImageIsEncoded(const Image& image) const;
};
