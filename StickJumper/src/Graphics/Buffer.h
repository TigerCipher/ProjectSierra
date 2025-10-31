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
#include "Vertex.h"

#include <vector>
#include <string>

namespace stick
{

enum class ShaderDataType : u8
{
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

struct BufferElement
{
    std::string    Name;
    ShaderDataType Type;
    u32            Size;
    size_t         Offset = 0;
    bool           Normalized;

    BufferElement() = default;
    BufferElement(ShaderDataType type, std::string name, bool normalized = false);
};


class BufferLayout
{
public:
    BufferLayout() = default;
    BufferLayout(const std::initializer_list<BufferElement>& elements) : _elements(elements) { CalculateOffsetsAndStride(); }

    [[nodiscard]] const std::vector<BufferElement>& Elements() const { return _elements; }

    [[nodiscard]] constexpr u32 Stride() const { return _stride; }

private:
    std::vector<BufferElement> _elements{};
    u32                        _stride = 0;

    void CalculateOffsetsAndStride();
};

class VertexBuffer
{
public:
    VertexBuffer(const f32* data, u32 size);
    VertexBuffer(const Vertex* data, u32 count);
    virtual ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    void SetData(const f32* data, u32 size) const;
    void SetData(const Vertex* data, u32 size) const;
    
    void SetLayout(const BufferLayout& layout);

    [[nodiscard]] const BufferLayout& Layout() const { return _layout; }

    [[nodiscard]] constexpr u32 Count() const { return _count; }
    
private:
    u32 _id = 0;
    BufferLayout _layout;
    u32 _count = 0;
};

class IndexBuffer
{
public:
    IndexBuffer(const u32* indices, u32 count);
    virtual ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    [[nodiscard]] constexpr u32 Count() const { return _count; }
    
private:
    u32 _id = 0;
    u32 _count = 0;
};

} // namespace stick