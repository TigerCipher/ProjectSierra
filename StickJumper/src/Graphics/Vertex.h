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

#include "Types.h"
#include <glm/glm.hpp>

namespace stick
{

struct Vertex
{
    glm::vec3 Position{};
    glm::vec4 Color{};
    glm::vec2 TexCoord{};
    f32       TexIndex = 0.0f;
};

} // namespace stick