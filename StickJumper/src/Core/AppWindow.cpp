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

#include "AppWindow.h"

#include "Graphics/Camera.h"
#include "Graphics/GLDebug.h"
#include "InputManager.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace stick
{

namespace
{

// cpp
void CheckSwapControlSupportAndSet(bool useVsync)
{
    GLint numExt = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);

    std::string extList;
    for (GLint i = 0; i < numExt; ++i)
    {
        if (const auto e = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)))
        {
            if (!extList.empty()) extList += ' ';
            extList += e;
        }
    }

    bool hasSwapControlTear = extList.find("WGL_EXT_swap_control_tear") != std::string::npos ||
                              extList.find("GLX_EXT_swap_control_tear") != std::string::npos ||
                              extList.find("EXT_swap_control_tear") != std::string::npos;

    LOG_DEBUG("Supports adaptive vsync: {}", hasSwapControlTear);

    // -1 for adaptive vsync (requires tear support)
    const i32 vsyncType = useVsync ? (hasSwapControlTear ? -1 : 1) : 0;
    glfwSwapInterval(vsyncType);
}


}

AppWindow::~AppWindow()
{
    Destroy();
}

void AppWindow::Init()
{
    LOG_INFO("Initializing window");
    if (!glfwInit())
    {
        throw StickException("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    LOG_DEBUG("Creating window handle");
    _windowPtr = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

    if (!_windowPtr)
    {
        glfwTerminate();
        throw StickException("Failed to create GLFW window");
    }

    LOG_DEBUG("Creating OpenGL context");
    glfwMakeContextCurrent(_windowPtr);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwDestroyWindow(_windowPtr);
        glfwTerminate();
        _windowPtr = nullptr;
        throw StickException("Failed to initialize GLAD OpenGL Loader");
    }

    EnableGLDebug(DebugSeverityLevel::Verbose, true);

    CheckSwapControlSupportAndSet(_AppSettings.UseVsync);

    glfwSetFramebufferSizeCallback(_windowPtr, [](GLFWwindow* window, i32 width, i32 height) {
        glViewport(0, 0, width, height);

        const auto data = (WindowData*)glfwGetWindowUserPointer(window);
        if (!data) return;

        if (data->WorldCamera)
        {
            data->WorldCamera->Resize(width, height, data->VirtualWorldHeight);
        }

        if (data->UiCamera)
        {
            // TODO make more like 0, 0 bottom right and width,height top right
            data->UiCamera->Resize(width, height, data->VirtualWorldHeight);
        }
    });

    LOG_INFO("Successfully initialized the window");

    InputHandler::Init(_windowPtr);
    
    _initialized = true;
}

void AppWindow::Destroy() noexcept
{
    if (!_initialized)
        return;
    LOG_INFO("Destroying window");
    
    glfwDestroyWindow(_windowPtr);
    glfwTerminate();
    _windowPtr = nullptr;
    _initialized = false;

    if (_windowData)
    {
        delete _windowData;
        _windowData = nullptr;
    }
}

bool AppWindow::ShouldClose() const noexcept
{
    return glfwWindowShouldClose(_windowPtr);
}

void AppWindow::SwapBuffers() const noexcept
{
    glfwSwapBuffers(_windowPtr);
}

void AppWindow::PollEvents() noexcept
{
    glfwPollEvents();
}
void AppWindow::SetTitle(const std::string& title) const noexcept
{
    glfwSetWindowTitle(_windowPtr, title.c_str());
}

void AppWindow::SetSize(const i32 width, const i32 height) noexcept
{
    _width = width;
    _height = height;
}

void AppWindow::SetWindowData(WindowData* data)
{
    _windowData = data;
    glfwSetWindowUserPointer(_windowPtr, _windowData);
}



} // namespace stick