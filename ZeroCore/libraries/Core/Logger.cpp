#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Zero {
    Ref<spdlog::logger> Log::s_ClientLogger;
    Ref<spdlog::logger> Log::s_CoreLogger;
    Ref<LogSink> Log::s_LogSink;

    void Log::Initialize() {
        spdlog::set_pattern("%^[%T] %n(%L): %v %$");

        s_LogSink = CreateRef<LogSink>();

        s_CoreLogger = CreateRef<spdlog::logger>("ZERO", spdlog::sinks_init_list{
            CreateRef<spdlog::sinks::stdout_color_sink_mt>(),
            s_LogSink
            });
        s_CoreLogger->set_level(spdlog::level::trace);

        s_ClientLogger = CreateRef<spdlog::logger>("APP", spdlog::sinks_init_list{
            CreateRef<spdlog::sinks::stdout_color_sink_mt>(),
            s_LogSink
            });
        s_ClientLogger->set_level(spdlog::level::trace);

        ZERO_CORE_LOG_INFO("Logger initialized successfully");



    }
}
