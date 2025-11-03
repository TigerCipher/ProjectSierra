// **************************************************************************** //
// Copyright 2025 Matt Rogers
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **************************************************************************** //

#pragma once

#include "Common.h"
#include <glm/glm.hpp>
#include <unordered_map>

struct GLFWwindow;

namespace stick
{
class InputHandler
{
public:
    static void Init(GLFWwindow* window);
    static void Update();

    static bool IsKeyDown(const i32 key) { return _keys[key]; }
    static bool IsKeyPressed(const i32 key) { return _keys[key] && !_lastKeys[key]; }
    static bool IsKeyReleased(const i32 key) { return !_keys[key] && _lastKeys[key]; }
    static bool IsMouseButtonDown(const i32 button) { return _mouseButtons[button]; }
    static bool IsMouseButtonPressed(const i32 button) { return _mouseButtons[button] && !_lastMouseButtons[button]; }
    static bool IsMouseButtonReleased(const i32 button) { return !_mouseButtons[button] && _lastMouseButtons[button]; }
    static const glm::vec2& GetMousePosition() { return _mousePosition; }
    static const glm::vec2& GetMouseDelta() { return _mouseDelta; }
    static f32              GetScrollDelta() { return _scrollDelta; }

    static void SetCursorLocked(bool locked);

private:
    static inline GLFWwindow* _window = nullptr;

    static inline std::unordered_map<i32, bool> _keys{};
    static inline std::unordered_map<i32, bool> _lastKeys{};
    static inline std::unordered_map<i32, bool> _mouseButtons{};
    static inline std::unordered_map<i32, bool> _lastMouseButtons{};

    static inline glm::vec2 _mousePosition{ 0.0f, 0.0f };
    static inline glm::vec2 _lastMousePosition{ 0.0f, 0.0f };
    static inline glm::vec2 _mouseDelta{ 0.0f, 0.0f };
    static inline bool _firstMouse = true;

    static inline f32 _scrollDelta = 0.0f;

    // Callbacks
    static void KeyCallback(GLFWwindow* window, i32 key, i32 scancode, i32 action, i32 mods);
    static void MouseButtonCallback(GLFWwindow* window, i32 button, i32 action, i32 mods);
    static void CursorPositionCallback(GLFWwindow* window, f64 x, f64 y);
    static void ScrollCallback(GLFWwindow* window, f64 xOffset, f64 yOffset);
};

// TODO: Handle mouse and controller actions as well
// TODO: Save bindings to settings.json
class InputManager
{
public:
    static void BindAction(const std::string& name, i32 key);
    static void UnbindAction(const std::string& name, i32 key);
    static void ClearAction(const std::string& name);
    static bool IsActionDown(const std::string& name);
    static bool IsActionPressed(const std::string& name);
    static bool IsActionReleased(const std::string& name);

    static void LogBindings();
private:
    static inline std::unordered_map<std::string, std::vector<i32>> _bindings;
};

} // namespace stick
