#include "App.h"

#include <glad/glad.h>

namespace stick
{

void App::CreateWindow(const std::string& title, const int width, const int height)
{
    _window = AppWindow(title, width, height);
    error_t error = _window.Init();
    if (!error)
    {
        _init = true;
    }
}

void App::Run()
{
    if (!_init) return;
    
    while (!_window.ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        _window.SwapBuffers();
        _window.PollEvents();
    }
}

} // namespace stick