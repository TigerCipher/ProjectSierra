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


#include "App.h"

#include <algorithm>
#include <glad/glad.h>
#include <chrono>
#include <thread>

#include "Util/AppSettingsSerializer.h"
#include "Util/SettingsManager.h"

namespace stick
{

namespace
{

SettingsManager _Settings("./app/settings.json");

} // anonymous namespace

void App::Init()
{
    _Settings.RegisterDefault("app", AppSettings());
    _Settings.Load();
    _AppSettings = _Settings.Get<AppSettings>("app");
}

void App::CreateWindow(const std::string& title, const int width, const int height)
{
    _window = CreateScope<AppWindow>(title, width, height);

    _window->Init();
    _init = true;

    _vao    = CreateScope<VertexArray>();
    _shader = CreateScope<Shader>("./assets/shaders/Basic.glsl");

    // @formatter:off
    f32 vertices[] = {
        //   x      y        r     g     b     a
        -0.5f, -0.5f,    1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,    0.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,    0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,    1.0f, 1.0f, 0.0f, 1.0f
    };

    u32 indices[] =  {
        0, 1, 2,
        2, 3, 0
    };
    
    // @formatter:on

    auto vb = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
    vb->SetLayout({
        { ShaderDataType::Float2, "a_Position" },
        { ShaderDataType::Float4, "a_Color" }
    });

    _indexBuffer = CreateRef<IndexBuffer>(indices, sizeof(indices) / sizeof(u32));

    _vao->AddVertexBuffer(vb);
    _vao->SetIndexBuffer(_indexBuffer);

    _shader->Bind();
    _shader->SetMat4("u_ViewProjection", glm::mat4(1.0f));
    _shader->SetMat4("u_Transform", glm::mat4(1.0f));

    _renderer = CreateScope<Renderer>();
}

void App::Run()
{
    if (!_init)
        return;
    LOG_INFO("Running App");

    using clock    = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<f32>;

    constexpr f32 FixedTimeStep = 1.0f / 60.0f; // 60 Hz logic updates
    auto          previous      = clock::now();

    f32  accumulator = 0.0f;
    i32  frameCount  = 0;
    f32  fps         = 0.0f;
    auto fpsTimer    = clock::now();

    while (!_window->ShouldClose())
    {
        constexpr f32 MaxAccumulatedTime = 0.25f;
        auto          now                = clock::now();
        f32           deltaTime          = std::chrono::duration_cast<duration>(now - previous).count();
        previous                         = now;
        accumulator += std::clamp(deltaTime, 0.0f, MaxAccumulatedTime);

        frameCount++;

        // Update FPS once per second
        if (std::chrono::duration_cast<std::chrono::seconds>(now - fpsTimer).count() >= 1)
        {
            fps             = static_cast<f32>(frameCount);
            frameCount      = 0;
            fpsTimer        = now;
            f32 frameTimeMs = deltaTime * 1000.0f;

            _window->SetTitle(std::format("Stick Jumper | FPS: {:.2f}", fps));
            LOG_DEBUG("FPS: {:.2f} | Delta: {:.4f} | Frame Time (MS): {:.4f}", fps, deltaTime, frameTimeMs);
            _renderer->LogFrameStats();
        }

        _window->PollEvents();
        // Handle input

        // Update tick - logic, physics, etc. This happens at a fixed timestep
        while (accumulator >= FixedTimeStep)
        {
            // Update logic
            accumulator -= FixedTimeStep;
        }

        // Rendering at variable timestep
        _renderer->BeginFrame();
        _renderer->Draw(_vao.get(), _shader.get());
        _renderer->EndFrame();

        // Present to screen
        _window->SwapBuffers();

        LimitFrameRate(now);
    }
}
void App::LimitFrameRate(const std::chrono::time_point<std::chrono::steady_clock> frameStart) const
{
    if (!_AppSettings.LimitFrameRate)
        return;

    const f32 targetFrameTime = 1.0f / _AppSettings.TargetFrameRate;
    using clock               = std::chrono::high_resolution_clock;

    const auto frameEnd      = clock::now();
    const f32  frameDuration = std::chrono::duration<f32>(frameEnd - frameStart).count();

    if (frameDuration < targetFrameTime)
    {
        f32 remaining = targetFrameTime - frameDuration;

        // Sleep coarsely if more than ~2ms remain
        if (remaining > 0.002f)
        {
            std::this_thread::sleep_for(std::chrono::duration<f32>(remaining - 0.001f));
        }

        // Busy-wait for fine precision
        while (std::chrono::duration<f32>(clock::now() - frameStart).count() < targetFrameTime)
            std::this_thread::yield();
    }
}

} // namespace stick