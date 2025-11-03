// **************************************************************************** //
//    Copyright 2025 Matt Rogers
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **************************************************************************** //


#include "InputManager.h"

#include <GLFW/glfw3.h>

namespace stick
{

void InputManager::Init(GLFWwindow* window)
{
    LOG_INFO("Initializing input manager");
    _window = window;
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void InputManager::Update()
{
    _lastKeys         = _keys;
    _lastMouseButtons = _mouseButtons;
}

void InputManager::KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
    if (action == GLFW_PRESS)
    {
        _keys[key] = true;
    } else if (action == GLFW_RELEASE)
    {
        _keys[key] = false;
    }
}

void InputManager::MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods)
{
    if (action == GLFW_PRESS)
    {
        _mouseButtons[button] = true;
    } else if (action == GLFW_RELEASE)
    {
        _mouseButtons[button] = false;
    }
}

void InputManager::CursorPositionCallback(GLFWwindow* window, f64 x, f64 y)
{
    const auto cursorPosition = glm::vec2(x, y);
    _mouseDelta               = cursorPosition - _lastMousePosition;
    _lastMousePosition        = _mousePosition;
    _mousePosition            = cursorPosition;
}

void InputManager::ScrollCallback(GLFWwindow* window, f64 xOffset, const f64 yOffset)
{
    _scrollDelta = static_cast<f32>(yOffset);
}

} // namespace stick