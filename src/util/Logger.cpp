#include "Logger.h"

void EDS::Logger::warning(const std::string& message) {
	emit loggedWarning(message);
}

void EDS::Logger::error(const std::string& message) {
	emit loggedError(message);
}