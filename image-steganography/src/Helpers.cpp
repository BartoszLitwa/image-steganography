#pragma once
#include "Helpers.hpp"

bool Helpers::endsWith(std::string const& value, std::string const& ending) {
    if (ending.size() > value.size()) 
        return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::vector<bool> Helpers::stringToBits(const std::string& msg) {
	std::vector<bool> bits;
	for (char c : msg) {
		for (int i = 7; i >= 0; i--) {
			bits.push_back((c >> i) & 1); // take the i-th bit of c
		}
	}
	return bits;
}

std::string Helpers::bitsToString(const std::vector<bool>& msg) {
	std::string str = "";
	for (int i = 0; i < msg.size(); i += 8) {
		char c = 0;
		for (int j = 0; j < 8; ++j) {
			// retrieve each bit and push it to the left of c
			// | is the bitwise OR operator
			c |= msg[i + j] << (7 - j);
		}
		str.push_back(c);
	}
	return str;
}