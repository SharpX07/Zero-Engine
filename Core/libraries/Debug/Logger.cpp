#include "logger.h"
#include <chrono>
#include <iomanip>

namespace Zero
{

	Logger::Logger()
	{
	}

	void Logger::logDebug(const std::string& message) const {
		log("DEBUG", message);
	}

	void Logger::logInfo(const std::string& message) const {
		log("INFO", message);
	}

	void Logger::logWarning(const std::string& message) const {
		log("WARNING", message);
	}

	void Logger::logError(const std::string& message) const {
		log("ERROR", message);
	}

	void Logger::log(const std::string& level, const std::string& message) const {
		std::cout << "[" << getCurrentTime() << "][" << level << "] " << message << std::endl;
	}

	std::string Logger::getCurrentTime() const {
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm* current_time = std::localtime(&now_c);

		// Formato personalizado de fecha y hora
		char buffer[80];
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", current_time);

		return std::string(buffer);
	}
}
