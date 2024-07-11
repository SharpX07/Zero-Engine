#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Zero
{
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	void Log::initialize()
	{
		spdlog::set_pattern("%^[%T] %n(%L): %v %$");
		s_CoreLogger = spdlog::stdout_color_mt("ZERO");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
		ZERO_CORE_LOG_INFO("Logger initialized successfully");
	}
}
