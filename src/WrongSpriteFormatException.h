#pragma once
#include <exception>
#include <string>
#include <string_view>

class WrongSpriteFormatException : std::exception {
public:
	WrongSpriteFormatException(const std::string expectedFormat, std::string_view spriteFile) : spriteFile(spriteFile) {
		message += expectedFormat;
	}

	inline const char* what() const noexcept override {
		return message.c_str();
	}

	inline std::string_view file() const noexcept {
		return spriteFile;
	}

private:
	std::string message = "Wrong sprite format! Expected format: ";

	std::string spriteFile;
};