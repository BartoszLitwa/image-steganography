#pragma once
#include <string>
#include <unordered_map>

enum Messages {
	MSG_UNSUPPORTED_FILE_FROMAT,
	MSG_FILE_NOT_FOUND,
	MSG_MISSING_FILEPATH,
	MSG_MISSING_FILEPATH_ARGUMENT,
	MSG_UNKNOWN_FLAG,
	MSG_UNABLE_TO_WRITE,
	MSG_UNABLE_TO_READ,
	MSG_UNABLE_TO_ENCODE,
	MSG_UNABLE_TO_DECODE,
	MSG_NOT_ENCODED,
	MSG_MISSING_MESSAGE_TO_ENCODE
};

enum FileType {
	BMP = 0x4D42,
	PNG = 0xD8FF,
	PPM = 0x5030,
};

const std::unordered_map<FileType, std::string> fileTypeToString = {
	{FileType::BMP, "BMP"},
	{FileType::PNG, "PNG"},
	{FileType::PPM, "PPM"}
};