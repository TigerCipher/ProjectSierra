#pragma once

#include "../Common.h"
#include "AppWindow.h"

namespace stick
{
class App
{
public:
    App() = default;
    virtual ~App() = default;

    void CreateWindow(const std::string& title, int width, int height);

    void Run();
    
private:
    AppWindow _window;
    bool _init = false;
};

}