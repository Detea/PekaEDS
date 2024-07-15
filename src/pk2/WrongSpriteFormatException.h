#pragma once
#include <exception>
#include <qstring.h>

class WrongSpriteFormatException : std::exception {
public:
	WrongSpriteFormatException(const QString& expectedFormat, const QString& spriteFile) : spriteFile(spriteFile) {
		message += expectedFormat;
	}

	inline const char* what() const noexcept override {
		return message.toStdString().c_str();
	}

	inline const QString& file() const noexcept {
		return spriteFile;
	}

private:
	QString message = "Wrong sprite format! Expected format: ";

	QString spriteFile;
};