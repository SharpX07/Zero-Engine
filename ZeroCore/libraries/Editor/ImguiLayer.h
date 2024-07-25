#pragma once
#include <Core/Layer.h>

namespace Zero
{
    class ImguiLayer : public Layer
    {
    public:
        ImguiLayer();
        ~ImguiLayer();
        void OnEvent(Event& event) override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnImGuiRender();       
        void Begin();
        void End();

    };
}