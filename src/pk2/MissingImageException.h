#pragma once
#include <exception>
#include <string>

class MissingImageException : public std::exception {
public:
	MissingImageException(std::string_view filename) : imageFile(filename) {
		message = std::string("Unable to load image file: ") + imageFile;
	}

	inline const char* what() const noexcept override {
		return message.c_str();
	}

private:
	std::string message;
	std::string imageFile;
};