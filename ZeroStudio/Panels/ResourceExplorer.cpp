#include "ResourceExplorer.h"
#include <ResourceManagement/ResourceManager.h>
#include <imgui.h>
#include <string>
#include <vector>
#include <memory>

namespace Zero
{
    void ResourcesExplorer::OnRender()
    {
        ImGui::Begin("Resource Explorer");

        // Usar un child para permitir desplazamiento si el contenido excede el tamaño de la ventana
        ImVec2 contentRegion = ImGui::GetContentRegionAvail();
        ImGui::BeginChild("##ResourceList", ImVec2(contentRegion.x, contentRegion.y), true);

        DrawResources();

        ImGui::EndChild();
        ImGui::End();
    }

    void ResourcesExplorer::DrawResourceThumbnail(const Ref<Resource>& resource, const ImVec2& size = ImVec2(64, 64))
    {
        if (!resource)
            return;

        // Obtén la miniatura del recurso
        auto thumbnail = false; // Implementa este método para obtener la miniatura

        ImVec2 cursorPos = ImGui::GetCursorScreenPos(); // Posición actual del cursor

        if (thumbnail)
        {
            //GLuint textureID = thumbnail->GetTextureID();
            //ImGui::Image(reinterpret_cast<void*>(textureID), size); // Dibuja una imagen de tamaño especificado
        }
        else
        {
            ImVec4 grayColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            // Dibuja un cuadro gris si no hay miniatura disponible
            if (dynamic_cast<Model*>(resource.get())) {
                // Es un Model
                grayColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            }
            else if (dynamic_cast<GLTexture*>(resource.get())) {
                grayColor = ImVec4(0.2f, 1.0f, 0.4f, 1.0f);
            }
            else if (dynamic_cast<Shader*>(resource.get())) {
                grayColor = ImVec4(0.0f, 0.2f, 1.0f, 1.0f);
            }
            
            ImGui::GetWindowDrawList()->AddRectFilled(cursorPos,
                ImVec2(cursorPos.x + size.x, cursorPos.y + size.y),
                ImGui::ColorConvertFloat4ToU32(grayColor));
            ImGui::Dummy(size); // Espacio reservado para mantener el layout
        }
    }

    void ResourcesExplorer::DrawResources()
    {
        const ImVec2 thumbnailSize(128, 128); // Tamaño de cada thumbnail
        const float padding = 10.0f; // Espaciado entre thumbnails
        float contentWidth = ImGui::GetContentRegionAvail().x; // Ancho disponible

        // Obtén todos los recursos
        const auto& resources = ResourceManager::GetInstance().GetAllResources();

        // Calcula el número de columnas
        int columns = static_cast<int>(contentWidth / (thumbnailSize.x + padding));
        columns = columns < 1 ? 1 : columns;

        ImGui::Columns(columns,nullptr,false);

        int index = 0;
        for (auto it = resources.begin(); it != resources.end(); ++it)
        {
            const Ref<Resource>& resource = it->second;

            ImGui::PushID(index);

            // Dibuja el thumbnail del recurso
            DrawResourceThumbnail(resource, thumbnailSize);

            // Opciones de contexto si se hace clic derecho
            if (ImGui::IsMouseClicked(1) && ImGui::IsItemHovered())
            {
                ImGui::OpenPopup("##resource_context_menu");
            }

            ImGui::TextWrapped("%s", resource->GetName().c_str());
            ImGui::PopID();
            ImGui::NextColumn();

            ++index;
        }


        ImGui::Columns(1);
    }
}
