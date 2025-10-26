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
#include <unordered_map>
#include <glm/glm.hpp>

namespace stick
{
class Shader
{
public:
    explicit Shader(std::string filePath);
    virtual ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetInt(const std::string& name, i32 value) const;
    void SetFloat(const std::string& name, f32 value) const;
    void SetVec2(const std::string& name, f32 value[2]) const;
    void SetVec2(const std::string& name, const glm::vec2& value) const;
    void SetVec3(const std::string& name, f32 value[3]) const;
    void SetVec3(const std::string& name, const glm::vec3& value) const;
    void SetVec4(const std::string& name, f32 value[4]) const;
    void SetVec4(const std::string& name, const glm::vec4& value) const;
    void SetMat4(const std::string& name, const glm::mat4& value) const;

private:
    u32                                  _id = 0;
    std::string                          _filePath;
    std::unordered_map<std::string, i32> _uniformLookup;
    bool                                 _isBound = false;

    [[nodiscard]] u32         CompileShader(const std::string& src, u32 type) const;
    [[nodiscard]] u32         CreateShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc) const;
    [[nodiscard]] std::string ReadFile() const;
    [[nodiscard]] i32         GetUniformLocation(const std::string& name);
};

} // namespace stick