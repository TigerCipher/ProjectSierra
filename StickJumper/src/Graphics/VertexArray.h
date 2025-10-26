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
#include "Buffer.h"

#include <vector>

namespace stick
{
class VertexArray
{
public:
    VertexArray();
    virtual ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const ref<IndexBuffer>& indexBuffer);

    [[nodiscard]] ref<IndexBuffer> GetIndexBuffer() const { return _indexBuffer; }
    
private:
    u32 _id = 0;
    u32 _vboIndex = 0;
    std::vector<ref<VertexBuffer>> _vertexBuffers{};
    ref<IndexBuffer> _indexBuffer = nullptr;
};

}