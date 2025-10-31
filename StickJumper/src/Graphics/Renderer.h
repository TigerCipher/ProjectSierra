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
#include "Texture.h"
#include "Camera.h"

namespace stick
{
class Renderer
{
public:
    Renderer(const ref<Camera>& camera);
    virtual ~Renderer() = default;

    void BeginFrame(const glm::vec3& clearColor = {0, 0, 0});
    void EndFrame();

    void BeginScene();
    void EndScene();
    void Flush();

    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, u32 zIndex = 0);
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture>& texture, const glm::vec4& tintColor = glm::vec4(1), u32 zIndex = 0, const glm::vec2* texCoords = nullptr);
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const ref<SubTexture>& texture, const glm::vec4& tintColor = glm::vec4(1), u32 zIndex = 0);

    void LogFrameStats() const;
private:

    struct Stats
    {
        u32 DrawCalls = 0;
        u32 VertexCount = 0;
        u32 IndexCount = 0;
        u32 QuadCount = 0;
        u32 TriangleCount = 0;
        u32 TextureCount = 0;
    } _stats;

    ref<Shader> _shader;
    ref<VertexArray> _quadVao;
    ref<VertexBuffer> _quadVbo;
    ref<IndexBuffer> _quadIbo;
    std::vector<Vertex> _vertexBufferBase{};
    u32 _quadCount = 0;

    ref<Camera> _camera = nullptr;

    static constexpr u32 MaxTextureSlots = 32;

    ref<Texture> _textureSlots[MaxTextureSlots];
    u32 _textureSlotIndex = 1; // 0 = white texture

    void Init();
};

}