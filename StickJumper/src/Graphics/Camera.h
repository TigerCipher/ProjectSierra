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
class Camera
{
public:
    explicit Camera(f32 left, f32 right, f32 bottom, f32 top);
    virtual ~Camera() = default;

    void SetPosition(const glm::vec3& position);
    void SetRotation(f32 rotation);
    void SetProjection(f32 left, f32 right, f32 bottom, f32 top);

    void Resize(i32 width, i32 height, f32 virtualHeight);

    [[nodiscard]] const glm::mat4& Projection() const { return _projection; }
    [[nodiscard]] const glm::mat4& View() const { return _view; }
    [[nodiscard]] const glm::mat4& ViewProjection() const { return _viewProjection; }
    [[nodiscard]] const glm::vec3& Position() const { return _position; }
    [[nodiscard]] constexpr f32    Rotation() const { return _rotation; }

private:
    glm::mat4 _projection{};
    glm::mat4 _view{ 1.0f };
    glm::mat4 _viewProjection{};

    glm::vec3 _position{ 0.0f, 0.0f, 0.0f };
    f32       _rotation = 0.0f;

    void RecalculateViewProjection();
};
} // namespace stick