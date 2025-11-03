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
#include "Shader.h"
#include "VertexArray.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Color.h"

namespace stick
{
class Renderer
{
public:
    Renderer()          = default;
    virtual ~Renderer() = default;

    void Init(const ref<Camera>& camera);

    void BeginFrame(const glm::vec3& clearColor = { 0, 0, 0 });

    void BeginScene();
    void EndScene();
    void Flush() const;

    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color_t& color, u32 zIndex = 0) const;
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture,
                  const Color_t& tintColor = Color_t(1), u32 zIndex = 0, const glm::vec2* texCoords = nullptr) const;
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const ref<SubTexture2D>& texture,
                  const Color_t& tintColor = Color_t(1), u32 zIndex = 0) const;

    void LogFrameStats() const;

private:
    static constexpr u32 MaxTextureSlots = 32;
    struct Stats
    {
        u32 DrawCalls     = 0;
        u32 VertexCount   = 0;
        u32 IndexCount    = 0;
        u32 QuadCount     = 0;
        u32 TriangleCount = 0;
        u32 TextureCount  = 0;
    } mutable _stats;

    mutable std::vector<Vertex> _vertexBufferBase{};
    mutable u32                 _quadCount = 0;
    mutable ref<Texture2D>      _textureSlots[MaxTextureSlots];
    mutable u32                 _textureSlotIndex = 1; // 0 = white texture

    ref<Camera>       _camera = nullptr;
    ref<Shader>       _shader;
    ref<VertexArray>  _quadVao;
    ref<VertexBuffer> _quadVbo;
    ref<IndexBuffer>  _quadIbo;

    void InitGraphics();
};

} // namespace stick