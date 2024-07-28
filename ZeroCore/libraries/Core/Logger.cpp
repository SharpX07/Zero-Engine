#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Zero
{
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	void Log::initialize()
	{
		spdlog::set_pattern("%^[%T] %n(%L): %v %$");

        s_LogSink = std::make_shared<LogSink>();

        s_CoreLogger = std::make_shared<spdlog::logger>("ZERO", spdlog::sinks_init_list{
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>(),
            s_LogSink
            });
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
		ZERO_CORE_LOG_INFO("Logger initialized successfully");
	}
}
