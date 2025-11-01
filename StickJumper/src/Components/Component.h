// **************************************************************************** //
// Copyright 2025 Matt Rogers
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **************************************************************************** //

#pragma once

#include "Common.h"
#include "Graphics/Renderer.h"

namespace stick
{

class Entity;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void Update(f32 deltaTime) {}
    virtual void Render(Renderer& renderer) {}

    void SetOwner(Entity* entity) { _owner = entity; }
    
    [[nodiscard]] virtual const char* Name() const = 0;

protected:
    Entity* _owner = nullptr;
};


struct TransformComponent : Component
{
    glm::vec2 Position = glm::vec2(0.0f);
    glm::vec2 Scale = glm::vec2(1.0f);
    f32 Rotation = 0.0f;

    TransformComponent() = default;
    explicit TransformComponent(const glm::vec2& position) : Position(position) {}
    TransformComponent(const glm::vec2& position, const glm::vec2& scale) : Position(position), Scale(scale) {}
    TransformComponent(const glm::vec2& position, const f32 rotation) : Position(position), Rotation(rotation){}
    TransformComponent(const glm::vec2& position, const glm::vec2& scale, const f32 rotation) : Position(position), Scale(scale), Rotation(rotation) {}
    
    [[nodiscard]] const char* Name() const override { return "Transform"; }
};

struct QuadComponent : Component
{
    glm::vec4 Color = glm::vec4(1.0f);
    
    QuadComponent() = default;
    explicit QuadComponent(const glm::vec4& color) : Color(color) {}

    [[nodiscard]] const char* Name() const override { return "Quad"; }
};

} // namespace stick
