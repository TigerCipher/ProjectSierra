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

#include <string>
#include <exception>
#include <format>

namespace stick
{

class StickException : public std::exception
{
public:
    explicit StickException(std::string message) : _message(std::format("Stick Exception: {}", std::move(message))) {}

    template<class... Args>
    explicit StickException(std::format_string<Args...> fmt, Args&&... args) :
        _message(std::format("Stick Exception: {}", std::format(fmt, std::forward<Args>(args)...)))
    {}

    [[nodiscard]] const char* what() const override { return _message.c_str(); }

private:
    std::string _message;
};

class IOException : public StickException
{
public:
    explicit IOException(std::string message) : StickException(std::format("IO Error: {}", std::move(message))) {}

    template<class... Args>
    explicit IOException(std::format_string<Args...> fmt, Args&&... args) :
        StickException(std::format("IO Error: {}", std::format(fmt, std::forward<Args>(args)...)))
    {}
};

class ShaderException : public StickException
{
public:
    explicit ShaderException(std::string type, u32 id, std::string message) :
        StickException(std::format("{} Shader (ID: {}) Error: {}", type, id, std::move(message)))
    {}

    template<class... Args>
    explicit ShaderException(std::string type, u32 id, std::format_string<Args...> fmt, Args&&... args) :
        StickException(
            std::format("{} Shader (ID: {}) Error: {}", std::move(type), id, std::format(fmt, std::forward<Args>(args)...)))
    {}
};

class GfxException : public StickException
{
public:
    explicit GfxException(std::string message) : StickException(std::format("Graphics Error: {}", std::move(message))) {}

    template<class... Args>
    explicit GfxException(std::format_string<Args...> fmt, Args&&... args) :
        StickException(std::format("Graphics Error: {}", std::format(fmt, std::forward<Args>(args)...)))
    {}
};


class SystemException : public StickException
{
public:
    explicit SystemException(std::string message) : StickException(std::format("System Error: {}", std::move(message))) {}

    template<class... Args>
    explicit SystemException(std::format_string<Args...> fmt, Args&&... args) :
        StickException(std::format("System Error: {}", std::format(fmt, std::forward<Args>(args)...)))
    {}
};

} // namespace stick