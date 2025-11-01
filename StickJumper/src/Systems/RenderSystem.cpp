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


#include "RenderSystem.h"

namespace stick
{


void RenderSystem::Render(Renderer& renderer, const EntityRegistry& registry)
{
    const auto quadEntities = registry.GetEntitiesWithComponents<Quad>();

    for (const auto& entity : quadEntities)
    {
        const auto& transform = registry.GetComponent<Transform>(entity);
        const auto& quad      = registry.GetComponent<Quad>(entity);

        renderer.DrawQuad(transform.Position, transform.Size, quad.Color, transform.ZIndex);
    }

    const auto spriteEntities = registry.GetEntitiesWithComponents<Sprite>();
    for (const auto& entity : spriteEntities)
    {
        const auto& transform = registry.GetComponent<Transform>(entity);
        const auto& sprite    = registry.GetComponent<Sprite>(entity);

        if (sprite.SubTexture)
        {
            renderer.DrawQuad(transform.Position, transform.Size, sprite.SubTexture, sprite.TintColor, transform.ZIndex);
        } else if (sprite.Texture)
        {
            renderer.DrawQuad(transform.Position, transform.Size, sprite.Texture, sprite.TintColor, transform.ZIndex);
        }
    }
}

} // namespace stick