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

u32 _DefaultTexture = 0;

f32 CalculateZPosition(const u32 zIndex, const glm::vec4& color)
{
    u32 z = zIndex;
    if (color.a < 1.0f && zIndex == 0)
    {
        z += 1;
    }
    return -static_cast<f32>(z) * LayerSpacing;
}

f32 CalculateZPosition(const u32 zIndex, const ref<Texture>& texture)
{
    u32 z = zIndex;
    if (texture->Channels() > 3 && zIndex == 0)
    {
        z += 1;
    }
    return -static_cast<f32>(z) * LayerSpacing;
}

void CreateDefaultWhiteTexture()
{
    constexpr u32 whitePixel = 0xFFFFFFFF;
    glGenTextures(1, &_DefaultTexture);
    glBindTexture(GL_TEXTURE_2D, _DefaultTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace

Renderer::Renderer()
{
    LOG_INFO("Initializing renderer");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    CreateDefaultWhiteTexture();

    // Vertex data: position (x, y, z), texCoord (u, v)
    f32 vertices[] = {
        // pos              // tex
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom-left
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top-right
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top-left
    };

    u32 indices[] = { 0, 1, 2, 2, 3, 0 };

    _quadVao       = CreateRef<VertexArray>();
    const auto vbo = CreateRef<VertexBuffer>(vertices, sizeof(vertices));

    vbo->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float2, "a_TexCoord" }
    });

    _quadVao->AddVertexBuffer(vbo);

    const auto ibo = CreateRef<IndexBuffer>(indices, sizeof(indices) / sizeof(u32));
    _quadVao->SetIndexBuffer(ibo);

    _shader = CreateRef<Shader>("assets/shaders/Texture.glsl");
}

void Renderer::BeginFrame(const glm::vec3& clearColor)
{
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _stats = {};
}

void Renderer::EndFrame() {}

void Renderer::BeginScene() const
{
    _shader->Bind();
}

void Renderer::EndScene() const
{
    _shader->Unbind();
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const u32 zIndex)
{
    constexpr auto model = glm::mat4(1.0f);

    const glm::mat4 transform =
        glm::translate(model, glm::vec3(position, CalculateZPosition(zIndex, color))) * glm::scale(model, glm::vec3(size, 1.0f));

    _shader->SetMat4("u_Transform", transform);
    _shader->SetVec4("u_Color", color);
    _shader->SetInt("u_UseTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _DefaultTexture);
    _shader->SetInt("u_Texture", 0);

    const auto indexCount = _quadVao->GetIndexBuffer()->Count();

    _quadVao->Bind();
    glDrawElements(GL_TRIANGLES, (i32) indexCount, GL_UNSIGNED_INT, nullptr);

    _stats.DrawCalls++;
    _stats.IndexCount += indexCount;

    const u32 triangles = indexCount / 3;
    _stats.TriangleCount += triangles;

    const u32 quads = indexCount / 6;
    _stats.QuadCount += quads;

    if (const auto& vtxBuffers = _quadVao->VertexBuffers(); !vtxBuffers.empty() && vtxBuffers[0])
    {
        _stats.VertexCount += vtxBuffers[0]->Count();
    }
}

void Renderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const ref<Texture>& texture, const glm::vec4& tintColor,
                        const u32 zIndex)
{
    assert(texture);
    constexpr auto  model     = glm::mat4(1.0f);
    const glm::mat4 transform = glm::translate(model, glm::vec3(position, CalculateZPosition(zIndex, texture))) *
                                glm::scale(model, glm::vec3(size, 1.0f));

    _shader->SetMat4("u_Transform", transform);
    _shader->SetVec4("u_Color", tintColor);
    _shader->SetInt("u_UseTexture", 1);

    texture->Bind(0);
    _shader->SetInt("u_Texture", 0);

    const auto indexCount = _quadVao->GetIndexBuffer()->Count();

    _quadVao->Bind();
    glDrawElements(GL_TRIANGLES, (i32) indexCount, GL_UNSIGNED_INT, nullptr);

    _stats.DrawCalls++;
    _stats.IndexCount += indexCount;
    _stats.TextureCount += 1;

    const u32 triangles = indexCount / 3;
    _stats.TriangleCount += triangles;

    const u32 quads = indexCount / 6;
    _stats.QuadCount += quads;

    if (const auto& vtxBuffers = _quadVao->VertexBuffers(); !vtxBuffers.empty() && vtxBuffers[0])
    {
        _stats.VertexCount += vtxBuffers[0]->Count();
    }
}


void Renderer::LogFrameStats() const
{
    LOG_DEBUG("Frame Stats: DrawCalls={}, Vertices={}, Indices={}, Quads={}, Triangles={}, Textures={}", _stats.DrawCalls, _stats.VertexCount,
              _stats.IndexCount, _stats.QuadCount, _stats.TriangleCount, _stats.TextureCount);
}

} // namespace stick