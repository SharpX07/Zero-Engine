#pragma once

#include <spdlog/sinks/base_sink.h>
#include <imgui.h>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <spdlog/pattern_formatter.h>

class LogSink : public spdlog::sinks::base_sink<std::mutex>
{
public:
	LogSink(size_t max_buffer = 100) : max_buffer_(max_buffer)
	{
		set_formatter(std::make_unique<spdlog::pattern_formatter>("[%T] %n(%L): %v", spdlog::pattern_time_type::local));
	}

protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		spdlog::memory_buf_t formatted;
		formatter_->format(msg, formatted);
		buffer_.push_back({ fmt::to_string(formatted), msg.level });

		if (buffer_.size() > max_buffer_)
			buffer_.erase(buffer_.begin());
	}

	void flush_() override {}

public:
	const std::vector<std::pair<std::string, spdlog::level::level_enum>>& buffer() const { return buffer_; }

private:
	std::vector<std::pair<std::string, spdlog::level::level_enum>> buffer_;
	size_t max_buffer_;
};