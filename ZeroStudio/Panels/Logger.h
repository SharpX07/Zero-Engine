#pragma once

#include "imgui.h"
#include <spdlog/sinks/base_sink.h>
#include <mutex>
#include <vector>
#include <string>

namespace Zero {

    template<typename Mutex>
    class ImGuiSink : public spdlog::sinks::base_sink<Mutex> {
    public:
        ImGuiSink() {
            clear();
        }

        void clear() {
            std::lock_guard<Mutex> lock(this->mutex_);
            buffer.clear();
            lineOffsets.clear();
            lineOffsets.push_back(0);
        }

        void render() {
            std::lock_guard<Mutex> lock(this->mutex_);
            if (ImGui::Begin("Logger")) {
                ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::TextUnformatted(buffer.begin());
                if (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                    ImGui::SetScrollHereY(1.0f);
                ImGui::EndChild();
                if (ImGui::Button("Clear")) {
                    clear();
                }
                ImGui::Checkbox("Auto-scroll", &autoScroll);
            }
            ImGui::End();
        }

    protected:
        void sink_it_(const spdlog::details::log_msg& msg) override {
            std::lock_guard<Mutex> lock(this->mutex_);
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
            buffer.append(fmt::to_string(formatted));

            for (auto it = formatted.begin(); it != formatted.end(); ++it) {
                if (*it == '\n')
                    lineOffsets.push_back(buffer.size());
            }
        }

        void flush_() override {}

    private:
        ImGuiTextBuffer buffer;
        ImVector<int> lineOffsets;
        bool autoScroll = true;
    };

    using ImGuiLogger = ImGuiSink<std::mutex>;

} // namespace Zero
