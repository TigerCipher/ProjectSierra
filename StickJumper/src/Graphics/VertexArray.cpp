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


#include "VertexArray.h"

#include <glad/glad.h>

namespace stick
{

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &_id);
    LOG_DEBUG("Vertex Array created: ID={}", _id);
}

VertexArray::~VertexArray()
{
    LOG_DEBUG("Vertex Array destroyed: ID={}", _id);
    glDeleteVertexArrays(1, &_id);
}

void VertexArray::Bind() const
{
    glBindVertexArray(_id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer)
{
    Bind();
    vertexBuffer->Bind();
    const auto& layout   = vertexBuffer->Layout();
    const auto& elements = layout.Elements();

    for (const auto& element : elements)
    {
        GLenum glType = GL_FLOAT;
        u32    count  = 0;

        switch (element.Type)  // NOLINT(clang-diagnostic-switch-enum)
        {
        case ShaderDataType::Float: count = 1; break;
        case ShaderDataType::Float2: count = 2; break;
        case ShaderDataType::Float3: count = 3; break;
        case ShaderDataType::Float4: count = 4; break;
        default: throw GfxException("Unsupported ShaderDataType in VertexBuffer");
        }

        glEnableVertexAttribArray(_vboIndex);
        glVertexAttribPointer(_vboIndex,
            static_cast<i32>(count),
            glType,
            element.Normalized ? GL_TRUE : GL_FALSE,
            static_cast<i32>(layout.Stride()),
            (const void*)(element.Offset));
        
        ++_vboIndex;
    }

    _vertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const ref<IndexBuffer>& indexBuffer)
{
    Bind();
    indexBuffer->Bind();
    _indexBuffer = indexBuffer;
}

} // namespace stick