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


#include "Buffer.h"

#include <glad/glad.h>

namespace stick
{

namespace
{

u32 ShaderDataTypeSize(const ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float: return 4;
    case ShaderDataType::Float2: return 4 * 2;
    case ShaderDataType::Float3: return 4 * 3;
    case ShaderDataType::Float4: return 4 * 4;
    case ShaderDataType::Mat3: return 4 * 3 * 3;
    case ShaderDataType::Mat4: return 4 * 4 * 4;
    case ShaderDataType::Int: return 4;
    case ShaderDataType::Int2: return 4 * 2;
    case ShaderDataType::Int3: return 4 * 3;
    case ShaderDataType::Int4: return 4 * 4;
    case ShaderDataType::Bool: return 1;
    }

    throw GfxException(std::format("Unknown ShaderDataType: {}", static_cast<u8>(type)));
}
} // namespace

BufferElement::BufferElement(const ShaderDataType type, std::string name, const bool normalized) :
    Name(std::move(name)), Type(type), Size(ShaderDataTypeSize(type)), Normalized(normalized)
{}

void BufferLayout::CalculateOffsetsAndStride()
{
    size_t offset = 0;
    _stride       = 0;
    for (auto& element : _elements)
    {
        element.Offset = offset;
        offset += element.Size;
        _stride += element.Size;
    }
}

VertexBuffer::VertexBuffer(const void* data, u32 size)
{
    glCreateBuffers(1, &_id);
    glBindBuffer(GL_ARRAY_BUFFER, _id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    LOG_DEBUG("Vertex Buffer created: ID={}", _id);
}

VertexBuffer::~VertexBuffer()
{
    LOG_DEBUG("Vertex Buffer destroyed: ID={}", _id);
    glDeleteBuffers(1, &_id);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const void* data, const u32 size) const
{
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

IndexBuffer::IndexBuffer(const u32* indices, u32 count) : _count(count)
{
    glCreateBuffers(1, &_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)
    LOG_DEBUG("Index Buffer created: ID={}", _id);
}

IndexBuffer::~IndexBuffer()
{
    LOG_DEBUG("Index Buffer destroyed: ID={}", _id);
    glDeleteBuffers(1, &_id);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace stick