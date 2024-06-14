#include "Input.h"
#include <Core/Application.h>
namespace Zero
{
    float Input::m_ScrollDelta = 0;
    bool Input::m_MouseScrolled = false;
    glm::vec2 Input::GetMousePosition()
    {
        auto window = Application::GetInstance()->GetGlfwWindow();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return glm::vec2(static_cast<float>(xPos), static_cast<float>(yPos));
    }
    bool Input::KeyPressed(KeyCode key)
    {
        auto window = Application::GetInstance()->GetGlfwWindow();
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS;
    }
    bool Input::MousePressed(MouseCode code)
    {
        auto window = Application::GetInstance()->GetGlfwWindow();
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(code));
        return state == GLFW_PRESS;
    }

    bool Input::MouseReleased(MouseCode code)
    {
        auto window = Application::GetInstance()->GetGlfwWindow();
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(code));
        return state == GLFW_RELEASE;
    }
    float Input::GetMouseScrollDelta()
    {
        auto window = Application::GetInstance()->GetGlfwWindow();
        return m_ScrollDelta;
    }
    bool Input::MouseScrolled()
    {
        bool scrl = m_MouseScrolled;
        m_MouseScrolled = false;
        return scrl;
    }
}

