#pragma once
#include <string>
#include <vector>

static class Helpers {
public:
	static bool endsWith(const std::string& value, const std::string& ending);
	static std::vector<bool> stringToBits(const std::string& msg);
	static std::string bitsToString(const std::vector<bool>& msg);
};
