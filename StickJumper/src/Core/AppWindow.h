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

#pragma once
#include "Common.h"
#include <string>

struct GLFWwindow;
namespace stick
{

class Camera;

struct WindowData
{
    ref<Camera> WorldCamera = nullptr;
    ref<Camera> UiCamera = nullptr;
    f32 VirtualWorldHeight = 512.0f;
};

class AppWindow
{
public:
    AppWindow(std::string title, const i32 width, const i32 height) : _title(std::move(title)), _width(width), _height(height) {}
    virtual ~AppWindow();

    void Init();
    void Destroy() noexcept;

    [[nodiscard]] bool ShouldClose() const noexcept;

    void SwapBuffers() const noexcept;
    void PollEvents() noexcept;

    void SetTitle(const std::string& title) const noexcept;
    void SetSize(i32 width, i32 height) noexcept;

    void SetWindowData(WindowData* data);

    [[nodiscard]] const std::string& Title() const noexcept { return _title; }
    [[nodiscard]] i32 Width() const noexcept { return _width; }
    [[nodiscard]] i32 Height() const noexcept { return _height; }

private:
    std::string _title;
    i32         _width       = 600;
    i32         _height      = 480;
    GLFWwindow* _windowPtr   = nullptr;
    bool        _initialized = false;
    WindowData* _windowData  = nullptr;
};

} // namespace stick