#pragma once

#include <iostream>

namespace Zero
{
	class Logger {
	public:
		Logger();

		void logDebug(const std::string& message) const;
		void logInfo(const std::string& message) const;
		void logWarning(const std::string& message) const;
		void logError(const std::string& message) const;

	private:
		std::string output;

		void log(const std::string& level, const std::string& message) const;
		std::string getCurrentTime() const;
	};
}

