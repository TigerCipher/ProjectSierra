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
#include <chrono>
#include <thread>

#include "Util/AppSettingsSerializer.h"
#include "Util/SettingsManager.h"

#include "Components/Component.h"
#include "Systems/RenderSystem.h"
#include "InputManager.h"

#include <__msvc_ranges_to.hpp>
#include <GLFW/glfw3.h>

namespace stick
{

namespace
{

constexpr f32 PixelsToWorld = 1.0f / 256.0f; // 256 pixels = 1 world unit

SettingsManager _Settings("./app/settings.json");

} // anonymous namespace

void App::Init()
{
    _Settings.RegisterDefault("app", AppSettings());
    _Settings.Load();
    _AppSettings = _Settings.Get<AppSettings>("app");

    InputManager::BindAction("jump", GLFW_KEY_SPACE);
    InputManager::BindAction("jump", GLFW_KEY_W);
    InputManager::BindAction("move_left", GLFW_KEY_A);
    InputManager::BindAction("move_right", GLFW_KEY_D);
}

void App::CreateWindow(const std::string& title, const int width, const int height)
{
    _window = CreateScope<AppWindow>(title, width, height);

    _window->Init();
    _init = true;

    _texture     = CreateRef<Texture2D>("./assets/textures/test.png");
    _spriteSheet = CreateRef<Texture2D>("./assets/textures/spritesheet.png");
    _sprite      = CreateRef<SubTexture2D>(_spriteSheet, glm::vec2(1, 0), 32);

    constexpr f32 orthoHeight = 512.0f; // total world units tall
    const f32     aspect      = static_cast<f32>(width) / static_cast<f32>(height);
    const f32     orthoWidth  = orthoHeight * aspect;
    _camera                   = CreateRef<Camera>(-orthoWidth * 0.5f, orthoWidth * 0.5f, -orthoHeight * 0.5f, orthoHeight * 0.5f);

    _renderer.Init(_camera);

    _window->SetWindowData(new WindowData{ .WorldCamera = _camera });

    auto& player = _entityRegistry.CreateEntity(Transform(glm::vec2{200, -250}, glm::vec2{64, 64}));
    _entityRegistry.AddComponent<Sprite>(player, _sprite);
    // _camera->SetPosition(_entityRegistry.GetTransform(player).Position);

    auto& pinkQuad = _entityRegistry.CreateEntity();
    _entityRegistry.GetTransform(pinkQuad) = Transform(glm::vec2{-100, 0}, glm::vec2{64, 64});
    _entityRegistry.AddComponent<Quad>(pinkQuad, colors::Magenta);

    auto& texturedQuad = _entityRegistry.CreateEntity();
    _entityRegistry.GetTransform(texturedQuad) = Transform(glm::vec2{0, 100}, glm::vec2{64, 64});
    _entityRegistry.AddComponent<Sprite>(texturedQuad, _texture);

    auto& texturedQuad2 = _entityRegistry.CreateEntity();
    _entityRegistry.GetTransform(texturedQuad2) = Transform(glm::vec2{-100, 0}, glm::vec2{64, 64}, 5u);
    _entityRegistry.AddComponent<Sprite>(texturedQuad2, _texture, glm::vec4{0.0f, 1.0f, 0.3f, 1.0f});

    InputManager::LogBindings();
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
            _renderer.LogFrameStats();
        }

        _window->PollEvents();
        // Handle input
         if (InputManager::IsActionPressed("jump"))
        {
            LOG_INFO("Space pressed");
        }

        if (InputHandler::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
        {
            LOG_INFO("Mouse clicked at {}, {}", InputHandler::GetMousePosition().x, InputHandler::GetMousePosition().y);
        }

        // auto mouseDelta = InputHandler::GetMouseDelta();
        // if (glm::length(mouseDelta) > 0.0f)
        // {
        //     LOG_INFO("Mouse moved by {:.4f}, {:.4f}", mouseDelta.x, mouseDelta.y);
        // }

        // Update tick - logic, physics, etc. This happens at a fixed timestep
        while (accumulator >= FixedTimeStep)
        {
            // Update logic
            accumulator -= FixedTimeStep;
        }

        // Rendering at variable timestep
        _renderer.BeginFrame({ 0.3f, 0.1f, 0.4f });
        _renderer.BeginScene();

        RenderSystem::Render(_renderer, _entityRegistry);
        
        _renderer.EndScene();

        // Present to screen
        _window->SwapBuffers();

        InputHandler::Update();

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