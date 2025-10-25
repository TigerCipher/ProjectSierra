#pragma once
#include "Common.h"
#include <string>

struct GLFWwindow;
namespace stick
{
class AppWindow
{
public:
    AppWindow() = default;
    AppWindow(std::string title, const int width, const int height) : _title(std::move(title)), _width(width), _height(height) {}
    virtual ~AppWindow();

    void Init();
    void Destroy() noexcept;

    [[nodiscard]] bool ShouldClose() const noexcept;
    void SwapBuffers() const noexcept;
    void PollEvents() noexcept;

private:
    std::string _title;
    int         _width     = 600;
    int         _height    = 480;
    GLFWwindow* _windowPtr = nullptr;
    bool        _destroyed = false;
};

} // namespace stick