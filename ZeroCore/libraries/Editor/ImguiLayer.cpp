#include "ImguiLayer.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <core/Application.h>

namespace Zero
{
    ImguiLayer::ImguiLayer() {}
    ImguiLayer::~ImguiLayer() {}
    void ImguiLayer::OnAttach()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        Application *app = Application::GetInstance();
        auto windowHandler = app->GetGlfwWindow();
        ImGui_ImplGlfw_InitForOpenGL(windowHandler, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }
    void ImguiLayer::OnDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void ImguiLayer::OnUpdate(float deltaTime)
    {
    }
    void ImguiLayer::OnImGuiRender()
    {
    }
    void ImguiLayer::OnEvent(Event &event)
    {
    }

    void ImguiLayer::Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();
    }
    void ImguiLayer::End()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

}