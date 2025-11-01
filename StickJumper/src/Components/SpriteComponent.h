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
#include "Component.h"
#include "Entity.h"

namespace stick
{
struct SpriteComponent : Component
{
    ref<SubTexture> Sprite;
    glm::vec4 TintColor = glm::vec4(1.0f);

    SpriteComponent() = default;
    explicit SpriteComponent(const ref<SubTexture>& sprite) : Sprite(sprite) {}
    SpriteComponent(const ref<SubTexture>& sprite, const glm::vec4& tintColor) : Sprite(sprite), TintColor(tintColor) {}

    [[nodiscard]] const char* Name() const override { return "Sprite"; }

    void Render(Renderer& renderer) override
    {
        const auto transform = _owner->GetComponent<TransformComponent>();
        if (!transform) return;
        renderer.DrawQuad(transform->Position, transform->Scale, Sprite, TintColor);
    }
};

} // namespace stick
