#pragma once

#include <string>
#include <exception>
#include <format>

namespace stick
{

class StickException : public std::exception
{
public:
    explicit StickException(std::string message) : _message(std::format("Stick Exception: {}", std::move(message))) {}

    [[nodiscard]] const char* what() const override { return _message.c_str(); }

private:
    std::string _message;
};

} // namespace stick