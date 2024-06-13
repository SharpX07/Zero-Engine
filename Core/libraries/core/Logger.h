#pragma once

#include <memory>
#include <spdlog/spdlog.h>
namespace Zero
{
	class Log {
	public:
		static void initialize();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#ifdef ZERO_CORE_LOGGER_ACTIVATE
#define ZERO_CORE_LOG_DEBUG(...) ::Zero::Log::GetCoreLogger()->debug(__VA_ARGS__);
#define ZERO_CORE_LOG_INFO(...) ::Zero::Log::GetCoreLogger()->info(__VA_ARGS__);
#define ZERO_CORE_LOG_WARN(...) ::Zero::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define ZERO_CORE_LOG_ERROR(...) ::Zero::Log::GetCoreLogger()->error(__VA_ARGS__);
#define ZERO_CORE_LOG_CRITICAL(...) ::Zero::Log::GetCoreLogger()->critical(__VA_ARGS__);
#else
#define ZERO_CORE_LOG_DEBUG(...) 
#define ZERO_CORE_LOG_INFO(...) 
#define ZERO_CORE_LOG_WARN(...) 
#define ZERO_CORE_LOG_ERROR(...) 
#define ZERO_CORE_LOG_CRITICAL(...) 

#endif 

#define ZERO_APP_LOG_DEBUG(...) ::Zero::Log::GetClientLogger()->debug(__VA_ARGS__);
#define ZERO_APP_LOG_INFO(...) ::Zero::Log::GetClientLogger()->info(__VA_ARGS__);
#define ZERO_APP_LOG_WARN(...) ::Zero::Log::GetClientLogger()->warn(__VA_ARGS__);
#define ZERO_APP_LOG_ERROR(...) ::Zero::Log::GetClientLogger()->error(__VA_ARGS__);
#define ZERO_APP_LOG_CRITICAL(...) ::Zero::Log::GetClientLogger()->critical(__VA_ARGS__);
