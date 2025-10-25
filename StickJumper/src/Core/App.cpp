#include "App.h"

#include <glad/glad.h>

#include <iostream>

namespace stick
{

void App::CreateWindow(const std::string& title, const int width, const int height)
{
    _window = AppWindow(title, width, height);

    try
    {
        _window.Init();
    } catch (const StickException& ex)
    {
        std::cerr << ex.what() << std::endl;
        throw;
    }
    _init = true;
}

void App::Run()
{
    if (!_init)
        return;

    while (!_window.ShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        _window.SwapBuffers();
        _window.PollEvents();
    }
}

} // namespace stick