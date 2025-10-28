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
#include "AppWindow.h"
#include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Renderer.h"

#include <chrono>

namespace stick
{
class App
{
public:
    App() = default;
    virtual ~App() = default;

    void Init();
    void CreateWindow(const std::string& title, int width, int height);

    void Run();
    
private:
    scope<AppWindow> _window;
    bool _init = false;
    scope<Renderer> _renderer = nullptr;
    ref<Texture> _texture = nullptr;
    ref<Texture> _spriteSheet = nullptr;
    ref<SubTexture> _sprite = nullptr;

    void LimitFrameRate(std::chrono::time_point<std::chrono::steady_clock> frameStart) const;
};

}