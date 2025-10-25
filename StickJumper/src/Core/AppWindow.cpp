#include "AppWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace stick
{

AppWindow::~AppWindow()
{
    Destroy();
}

void AppWindow::Init()
{
    if (!glfwInit())
    {
        _destroyed = true;
        throw StickException("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _windowPtr = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

    if (!_windowPtr)
    {
        glfwTerminate();
        _destroyed = true;
        throw StickException("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(_windowPtr);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        // TODO: Log error
        glfwDestroyWindow(_windowPtr);
        glfwTerminate();
        _windowPtr = nullptr;
        _destroyed = true;
        throw StickException("Failed to initialize GLAD OpenGL Loader");
    }
}

void AppWindow::Destroy() noexcept
{
    if (!_windowPtr && _destroyed)
        return;

    glfwDestroyWindow(_windowPtr);
    glfwTerminate();
    _windowPtr = nullptr;
    _destroyed = false;
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

} // namespace stick