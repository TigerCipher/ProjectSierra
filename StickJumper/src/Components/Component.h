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
#include "Graphics/Color.h"
#include "Graphics/Texture2D.h"

#include <glm/glm.hpp>

namespace stick
{

struct Transform
{
    glm::vec2 Position = glm::vec2(0.0f);
    glm::vec2 Size     = glm::vec2(1.0f);
    f32       Rotation = 0.0f;
    u32       ZIndex   = 0;

    explicit Transform() = default;
    Transform(const glm::vec2& position, const glm::vec2& scale, const f32 rotation = 0.0f, const u32 zIndex = 0) :
        Position(position), Size(scale), Rotation(rotation), ZIndex(zIndex)
    {}
    Transform(const glm::vec2& position, const f32 uniformScale, const f32 rotation = 0.0f, const u32 zIndex = 0) :
        Position(position), Size(glm::vec2(uniformScale)), Rotation(rotation), ZIndex(zIndex)
    {}
    Transform(const glm::vec2& position, const glm::vec2& size, const u32 zIndex) : Position(position), Size(size), ZIndex(zIndex)
    {}
    Transform(const glm::vec2& position, const f32 uniformScale, const u32 zIndex) :
        Position(position), Size(glm::vec2(uniformScale)), ZIndex(zIndex)
    {}
};

struct Sprite
{
    ref<Texture2D>    Texture    = nullptr;
    ref<SubTexture2D> SubTexture = nullptr;
    glm::vec4         TintColor  = glm::vec4(1.0f);

    explicit Sprite() = default;
    explicit Sprite(const ref<Texture2D>& texture, const glm::vec4& tintColor = glm::vec4(1.0f)) :
        Texture(texture), TintColor(tintColor)
    {}

    explicit Sprite(const ref<SubTexture2D>& subTexture, const glm::vec4& tintColor = glm::vec4(1.0f)) :
        SubTexture(subTexture), TintColor(tintColor)
    {}
};

struct Quad
{
    Color_t Color;

    explicit Quad() = default;
    explicit Quad(const glm::vec4& color) : Color(color) {}
};

} // namespace stick
