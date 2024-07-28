#include "LogPanel.h"
#include <Core/Logger.h>

namespace Zero
{
	void LogPanel::OnRender()
	{
		ImGui::Begin("Logs");

		auto& buffer = Log::GetImGuiLogSink()->buffer();

		for (const auto& [log, level] : buffer)
		{
			ImVec4 color;
			switch (level)
			{
			case spdlog::level::trace:
			case spdlog::level::debug:
				color = ImVec4(0.0f, 0.5f, 1.0f, 1.0f); // Celeste
				break;
			case spdlog::level::info:
				color = ImVec4(0.07f, 0.63f, 0.05f, 1.0f); // Verde
				break;
			case spdlog::level::warn:
				color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Amarillo
				break;
			case spdlog::level::err:
				color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Rojo
				break;
			case spdlog::level::critical:
				color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Rojo (podrías usar un fondo diferente para distinguirlo)
				break;
			default:
				color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Blanco por defecto
			}

			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextWrapped("%s", log.c_str());
			ImGui::PopStyleColor();
		}

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::End();
	}
}