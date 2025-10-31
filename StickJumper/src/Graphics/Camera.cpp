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


#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace stick
{

Camera::Camera(const f32 left, const f32 right, const f32 bottom, const f32 top)
{
    _projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    RecalculateViewProjection();
}

void Camera::SetPosition(const glm::vec3& position)
{
    _position = position;
    RecalculateViewProjection();
}

void Camera::SetRotation(const f32 rotation)
{
    _rotation = rotation;
    RecalculateViewProjection();
}

void Camera::SetProjection(const f32 left, const f32 right, const f32 bottom, const f32 top)
{
    _projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

void Camera::Resize(const i32 width, const i32 height, const f32 virtualHeight)
{
    const f32 aspect = static_cast<f32>(width) / static_cast<f32>(height);
    const f32 orthoHeight = virtualHeight;
    const f32 orthoWidth  = orthoHeight * aspect;

    SetProjection(
        -orthoWidth * 0.5f, orthoWidth * 0.5f,
        -orthoHeight * 0.5f, orthoHeight * 0.5f
    );
    RecalculateViewProjection();
}

void Camera::RecalculateViewProjection()
{
    const glm::mat4 transform =
        glm::translate(glm::mat4(1.0f), _position) * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation), glm::vec3(0, 0, 1));

    _view           = glm::inverse(transform);
    _viewProjection = _projection * _view;
}

} // namespace stick