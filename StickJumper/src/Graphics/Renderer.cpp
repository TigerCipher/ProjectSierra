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


#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace stick
{

namespace
{
constexpr f32 LayerSpacing = 0.001f;

constexpr u32 MaxQuads    = 1000;
constexpr u32 MaxVertices = MaxQuads * 4;
constexpr u32 MaxIndices  = MaxQuads * 6;


f32 CalculateZPosition(const u32 zIndex)
{
    return static_cast<f32>(zIndex) * LayerSpacing;
}

} // namespace


void Renderer::Init(const ref<Camera>& camera)
{
    LOG_INFO("Initializing renderer");
    InitGraphics();
    _vertexBufferBase = std::vector<Vertex>(MaxVertices);

    _quadVao = CreateRef<VertexArray>();
    _quadVbo = CreateRef<VertexBuffer>(_vertexBufferBase.data(), MaxVertices);

    _quadVbo->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4,    "a_Color" },
        { ShaderDataType::Float2, "a_TexCoord" },
        {  ShaderDataType::Float, "a_TexIndex" }
    });

    _quadVao->AddVertexBuffer(_quadVbo);

    // Index buffer
    std::vector<u32> indices(MaxIndices);

    u32 offset = 0;
    for (u32 i = 0; i < MaxQuads; i++)
    {
        indices[i * 6 + 0] = offset + 0;
        indices[i * 6 + 1] = offset + 1;
        indices[i * 6 + 2] = offset + 2;
        indices[i * 6 + 3] = offset + 2;
        indices[i * 6 + 4] = offset + 3;
        indices[i * 6 + 5] = offset + 0;
        offset += 4;
    }

    _quadIbo = CreateRef<IndexBuffer>(indices.data(), MaxIndices);

    _quadVao->SetIndexBuffer(_quadIbo);

    _shader = CreateRef<Shader>("assets/shaders/Texture.glsl");

    _camera = camera;
}

void Renderer::InitGraphics()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    u32 whiteTextureId;
    {
        constexpr u32 whitePixel = 0xFFFFFFFF;
        glCreateTextures(GL_TEXTURE_2D, 1, &whiteTextureId);
        glTextureStorage2D(whiteTextureId, 1, GL_RGBA8, 1, 1);
        glTextureSubImage2D(whiteTextureId, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &whitePixel);
    }

    for (int i = 0; i < 32; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, whiteTextureId);
    }
    
    _textureSlots[0] = CreateRef<Texture2D>(whiteTextureId);
}

void Renderer::BeginFrame(const glm::vec3& clearColor)
{
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _stats = {};
}

void Renderer::BeginScene()
{
    _quadCount = 0;
    _shader->Bind();
    _shader->SetMat4("u_ViewProj", _camera->ViewProjection());
}

void Renderer::EndScene()
{
    Flush();
    _shader->Unbind();
}

void Renderer::Flush() const
{
    if (_quadCount == 0)
        return;


    for (u32 i = 0; i < _textureSlotIndex; i++)
        _textureSlots[i]->Bind(i);

    int samplers[32];
    for (int i = 0; i < 32; i++)
        samplers[i] = i;
    _shader->SetIntArray("u_Texture", samplers, 32);

    _quadVbo->SetData(_vertexBufferBase.data(), _quadCount * 4 * sizeof(Vertex));
    _quadVao->Bind();

    glDrawElements(GL_TRIANGLES, _quadCount * 6, GL_UNSIGNED_INT, nullptr);

    _stats.DrawCalls++;
    _stats.IndexCount     = _quadCount * 6;
    _stats.TriangleCount = _stats.IndexCount / 3;
    _stats.QuadCount    = _quadCount;
    _stats.VertexCount = _stats.IndexCount / 6;
    _stats.TextureCount = _textureSlotIndex - 1;
    
    _quadCount        = 0;
    _textureSlotIndex = 1;
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color_t& color, const u32 zIndex) const
{
    DrawQuad(position, size, nullptr, color, zIndex, nullptr);
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture2D>& texture, const Color_t& tintColor,
                        const u32 zIndex, const glm::vec2* texCoords) const
{
    if (_quadCount > MaxQuads)
        Flush();

    float textureIndex = 0.0f; // Default to white texture
    if (texture)
    {
        for (u32 i = 1; i < _textureSlotIndex; ++i)
        {
            if (_textureSlots[i] == texture)
            {
                textureIndex = (float) i;
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            if (_textureSlotIndex >= MaxTextureSlots)
                Flush();

            textureIndex                     = (float) _textureSlotIndex;
            _textureSlots[_textureSlotIndex] = texture;
            _textureSlotIndex++;
        }
    }

    constexpr glm::vec2 defaultTexCoords[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f }
    };

    const glm::vec2* finalTexCoords = texCoords ? texCoords : defaultTexCoords;


    const f32 calculatedZ = CalculateZPosition(zIndex);

    const u32 vertexOffset = _quadCount * 4;

    _vertexBufferBase[vertexOffset + 0] = {
        .Position = { position.x, position.y, calculatedZ },
        .Color    = tintColor,
        .TexCoord = finalTexCoords[0],
        .TexIndex = textureIndex
    };

    _vertexBufferBase[vertexOffset + 1] = {
        .Position = { position.x + size.x, position.y, calculatedZ },
        .Color    = tintColor,
        .TexCoord = finalTexCoords[1],
        .TexIndex = textureIndex
    };

    _vertexBufferBase[vertexOffset + 2] = {
        .Position = { position.x + size.x, position.y + size.y, calculatedZ },
        .Color    = tintColor,
        .TexCoord = finalTexCoords[2],
        .TexIndex = textureIndex
    };

    _vertexBufferBase[vertexOffset + 3] = {
        .Position = { position.x, position.y + size.y, calculatedZ },
        .Color    = tintColor,
        .TexCoord = finalTexCoords[3],
        .TexIndex = textureIndex
    };

    _quadCount++;
}
void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const ref<SubTexture2D>& texture,
                        const Color_t& tintColor, const u32 zIndex) const
{
    DrawQuad(position, size, texture->Texture(), tintColor, zIndex, texture->TexCoords());
}


void Renderer::LogFrameStats() const
{
    LOG_DEBUG("Frame Stats: DrawCalls={}, Vertices={}, Indices={}, Quads={}, Triangles={}, Textures={}", _stats.DrawCalls,
              _stats.VertexCount, _stats.IndexCount, _stats.QuadCount, _stats.TriangleCount, _stats.TextureCount);
}


} // namespace stick