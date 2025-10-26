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

namespace stick
{

Renderer::Renderer()
{
    LOG_INFO("Initializing renderer");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void Renderer::BeginFrame(const glm::vec3& clearColor)
{
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _stats = {};
}

void Renderer::EndFrame()
{

}

void Renderer::Draw(const VertexArray& vertexArray, const Shader& shader, const glm::mat4& transform)
{
    shader.Bind();
    shader.SetMat4("u_Transform", transform);

    const auto indexCount = vertexArray.GetIndexBuffer()->Count();
    
    vertexArray.Bind();
    glDrawElements(GL_TRIANGLES, (i32)indexCount, GL_UNSIGNED_INT, nullptr);

    _stats.DrawCalls++;
    _stats.IndexCount += indexCount;

    const u32 triangles = indexCount / 3;
    _stats.TriangleCount += triangles;

    const u32 quads = indexCount / 6;
    _stats.QuadCount += quads;

    if (const auto& vtxBuffers = vertexArray.VertexBuffers(); !vtxBuffers.empty() && vtxBuffers[0])
    {
        _stats.VertexCount += vtxBuffers[0]->Count();
    }
}

void Renderer::Draw(const VertexArray* const vertexArray, const Shader* const shader, const glm::mat4& transform)
{
    Draw(*vertexArray, *shader, transform);
}

void Renderer::LogFrameStats() const
{
    LOG_DEBUG("Frame Stats: DrawCalls={}, Vertices={}, Indices={}, Quads={}, Triangles={}",
              _stats.DrawCalls,
              _stats.VertexCount,
              _stats.IndexCount,
              _stats.QuadCount,
              _stats.TriangleCount);
}

} // namespace stick