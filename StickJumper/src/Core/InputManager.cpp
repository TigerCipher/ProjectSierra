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

#include <algorithm>
#include <GLFW/glfw3.h>

namespace stick
{

void InputHandler::Init(GLFWwindow* window)
{
    LOG_INFO("Initializing input manager");
    _window = window;
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);
}

void InputHandler::Update()
{
    _mouseDelta = _mousePosition - _lastMousePosition;
    _lastMousePosition = _mousePosition;
    
    _lastKeys         = _keys;
    _lastMouseButtons = _mouseButtons;
}

void InputHandler::SetCursorLocked(bool locked)
{
    glfwSetInputMode(_window, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void InputHandler::KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods)
{
    if (action == GLFW_PRESS)
    {
        _keys[key] = true;
    } else if (action == GLFW_RELEASE)
    {
        _keys[key] = false;
    }
}

void InputHandler::MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods)
{
    if (action == GLFW_PRESS)
    {
        _mouseButtons[button] = true;
    } else if (action == GLFW_RELEASE)
    {
        _mouseButtons[button] = false;
    }
}

void InputHandler::CursorPositionCallback(GLFWwindow* window, f64 x, f64 y)
{
    _mousePosition = glm::vec2(x, y);
    if (_firstMouse)
    {
        _lastMousePosition = _mousePosition;
        _firstMouse        = false;
    }
}

void InputHandler::ScrollCallback(GLFWwindow* window, f64 xOffset, const f64 yOffset)
{
    _scrollDelta = static_cast<f32>(yOffset);
}

void InputManager::BindAction(const std::string& name, i32 key)
{
    if (auto& keys = _bindings[name]; std::ranges::find(keys, key) == keys.end())
    {
        keys.push_back(key);
    }
}
void InputManager::UnbindAction(const std::string& name, i32 key)
{
    auto& keys = _bindings[name];
    keys.erase(std::ranges::remove(keys, key).begin(), keys.end());
}

void InputManager::ClearAction(const std::string& name)
{
    _bindings.erase(name);
}

bool InputManager::IsActionDown(const std::string& name)
{
    const auto it = _bindings.find(name);
    if (it == _bindings.end())
        return false;


    return std::ranges::any_of(it->second.begin(), it->second.end(), [](auto& b) {
        return InputHandler::IsKeyDown(b);
    });
}

bool InputManager::IsActionPressed(const std::string& name)
{
    const auto it = _bindings.find(name);
    if (it == _bindings.end())
        return false;

    return std::ranges::any_of(it->second.begin(), it->second.end(), [](auto& b) {
        return InputHandler::IsKeyPressed(b);
    });
}

bool InputManager::IsActionReleased(const std::string& name)
{
    const auto it = _bindings.find(name);
    if (it == _bindings.end())
        return false;

    return std::ranges::any_of(it->second.begin(), it->second.end(), [](auto& b) {
        return InputHandler::IsKeyReleased(b);
    });
}

void InputManager::LogBindings()
{
    LOG_INFO("Current action bindings:");
    for (const auto& [action, keys] : _bindings)
    {
        std::string list;
        for (const i32 key : keys)
        {
            list += std::to_string(key) + " ";
        }
        LOG_INFO("  Action '{}': Keys [{}]", action, list);
    }
}

} // namespace stick