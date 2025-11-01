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
#include <glm/glm.hpp>

namespace stick
{
class Texture2D
{
public:
    explicit Texture2D(const u32 id) : _id(id), _width(1), _height(1) {}
    explicit Texture2D(std::string path, bool flipVertical = true);
    virtual ~Texture2D();

    void Bind(u32 slot = 0) const;
    void Unbind() const;

    [[nodiscard]] constexpr u32 Id() const { return _id; }
    [[nodiscard]] constexpr u32 Width() const { return _width; }
    [[nodiscard]] constexpr u32 Height() const { return _height; }

    [[nodiscard]] constexpr i32 Channels() const { return _channels; }

    bool operator==(const Texture2D& other) const { return _id == other._id; }
    bool operator!=(const Texture2D& other) const { return !(*this == other); }

private:
    u32         _id       = 0;
    i32         _width    = 0;
    i32         _height   = 0;
    i32         _channels = 0;
    std::string _path;
};


class SubTexture2D
{
public:
    SubTexture2D(const ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize,
                 const glm::vec2& spriteSize = { 1.0f, 1.0f });

    SubTexture2D(const ref<Texture2D>& texture, const glm::vec2& coords, f32 cellSize,
                 const glm::vec2& spriteSize = { 1.0f, 1.0f }) :
        SubTexture2D(texture, coords, { cellSize, cellSize }, spriteSize)
    {}

    virtual ~SubTexture2D() = default;

    [[nodiscard]] const ref<Texture2D>&      Texture() const { return _texture; }
    [[nodiscard]] constexpr const glm::vec2* TexCoords() const { return _texCoords; }

    [[nodiscard]] f32 Width() const { return _cellSize.x; }
    [[nodiscard]] f32 Height() const { return _cellSize.y; }

private:
    ref<Texture2D> _texture = nullptr;
    glm::vec2      _texCoords[4]{};
    glm::vec2      _cellSize{};
};

} // namespace stick