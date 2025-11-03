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


void RenderSystem::Render(const Renderer& renderer, const EntityRegistry& registry)
{
    registry.ForEach<Transform, Quad>(
        [&renderer](const Entity& e, const Transform& t, const Quad& q) { renderer.DrawQuad(t.Position, t.Size, q.Color, t.ZIndex); });

    registry.ForEach<Transform, Sprite>([&renderer](const Entity& e, const Transform& t, const Sprite& s) {
        if (s.SubTexture)
        {
            renderer.DrawQuad(t.Position, t.Size, s.SubTexture, s.TintColor, t.ZIndex);
        } else if (s.Texture)
        {
            renderer.DrawQuad(t.Position, t.Size, s.Texture, s.TintColor, t.ZIndex);
        }
    });
}

} // namespace stick