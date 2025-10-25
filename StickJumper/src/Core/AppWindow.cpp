#include "AppWindow.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace stick
{

AppWindow::~AppWindow()
{
    Destroy();
}

error_t AppWindow::Init()
{
    if (!glfwInit())
    {
        // TODO: Log error
        _destroyed = true;
        return Error::From(Error::WindowInit, Error::GlfwInit);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _windowPtr = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

    if (!_windowPtr)
    {
        // TODO: Log error
        glfwTerminate();
        _destroyed = true;
        return Error::WindowInit;
    }

    glfwMakeContextCurrent(_windowPtr);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        // TODO: Log error
        glfwDestroyWindow(_windowPtr);
        glfwTerminate();
        _windowPtr = nullptr;
        _destroyed = true;
        return Error::From(Error::WindowInit, Error::GladInit);
    }

    return Error::None;
}

void AppWindow::Destroy()
{
    if (!_windowPtr && _destroyed)
        return;

    glfwDestroyWindow(_windowPtr);
    glfwTerminate();
    _windowPtr = nullptr;
    _destroyed = false;
}

bool AppWindow::ShouldClose() const
{
    return glfwWindowShouldClose(_windowPtr);
}

void AppWindow::SwapBuffers() const
{
    glfwSwapBuffers(_windowPtr);
}

void AppWindow::PollEvents()
{
    glfwPollEvents();
}

} // namespace stick