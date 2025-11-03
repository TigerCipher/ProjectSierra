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
struct Color_t
{
    f32 R = 1.0f;
    f32 G = 1.0f;
    f32 B = 1.0f;
    f32 A = 1.0f;

    explicit Color_t() = default;
    explicit Color_t(const f32 uniform) : R(uniform), G(uniform), B(uniform) {}
    constexpr Color_t(const f32 r, const f32 g, const f32 b, const f32 a = 1.0f) : R(r), G(g), B(b), A(a) {}
    Color_t(const glm::vec4& vec) : R(vec.r), G(vec.g), B(vec.b), A(vec.a) {}

    // Conversion to glm::vec4
    operator glm::vec4() const { return { R, G, B, A }; }

    // conversion from glm::vec4
    Color_t& operator=(const glm::vec4& vec)
    {
        R = vec.r;
        G = vec.g;
        B = vec.b;
        A = vec.a;
        return *this;
    }
};


// Predefined Colors
namespace colors
{
constexpr auto White   = Color_t(1.0f, 1.0f, 1.0f, 1.0f);
constexpr auto Black   = Color_t(0.0f, 0.0f, 0.0f, 1.0f);
constexpr auto Red     = Color_t(1.0f, 0.0f, 0.0f, 1.0f);
constexpr auto Green   = Color_t(0.0f, 1.0f, 0.0f, 1.0f);
constexpr auto Blue    = Color_t(0.0f, 0.0f, 1.0f, 1.0f);
constexpr auto Yellow  = Color_t(1.0f, 1.0f, 0.0f, 1.0f);
constexpr auto Cyan    = Color_t(0.0f, 1.0f, 1.0f, 1.0f);
constexpr auto Magenta = Color_t(1.0f, 0.0f, 1.0f, 1.0f);
} // namespace colors

} // namespace stick
