#include "Input.h"
#include <Core/Application.h>
namespace Zero
{
    glm::vec2 Input::GetMousePosition()
    {
        return glm::vec2(0,0);
    }
    bool Input::KeyPressed(KeyCode key)
    {
        auto window = Application::GetInstance()->GetGlfwWindow();
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS;
    }
    bool Input::MousePressed(MouseCode code)
    {
        return false;
    }
}

