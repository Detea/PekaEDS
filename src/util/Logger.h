#pragma once
#include <qobject.h>

#include <string>

namespace EDS {
	class Logger : public QObject {
		Q_OBJECT

	public:
		void warning(const std::string& message);
		void error(const std::string& message);

	signals:
		void loggedWarning(const std::string& message);
		void loggedError(const std::string& message);
	};
}