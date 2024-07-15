#pragma once
#include <exception>
#include <string>

class WrongMapFormatException : std::exception {
public:
	WrongMapFormatException(const std::string& expectedFormat) {
		message += expectedFormat;
	}

	inline const char* what() const noexcept override {
		return message.c_str();
	}

private:
	std::string message = "Wrong map format! Expected format: ";
};