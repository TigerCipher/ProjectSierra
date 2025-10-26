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


#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace stick
{

namespace
{
void GetShaderBlocks(const std::string& src, std::string& vertexSrc, std::string& fragSrc)
{
    const std::string typeTag = "#type";

    size_t      pos = src.find(typeTag, 0);
    std::string block;

    while (pos != std::string::npos)
    {
        size_t eol = src.find_first_of("\r\n", pos);
        if (eol == std::string::npos)
            break;
        size_t      begin      = pos + typeTag.size() + 1;
        std::string shaderType = src.substr(begin, eol - begin);
        size_t      next       = src.find(typeTag, eol);
        size_t      codeStart  = src.find_first_not_of("\r\n", eol);

        std::string code = (next == std::string::npos) ? src.substr(codeStart) : src.substr(codeStart, next - codeStart);

        if (shaderType.find("vertex") != std::string::npos)
            vertexSrc = code;
        else if (shaderType.find("frag") != std::string::npos)
            fragSrc = code;
        else
            throw ShaderException("Unkown", -1, "Unknown shader type specified: " + shaderType);
        pos = next;
    }
}
} // namespace

Shader::Shader(std::string filePath) : _filePath(std::move(filePath))
{
    LOG_INFO("Loading shader from {}", _filePath);
    const std::string src = ReadFile();

    std::string vertexSrc, fragSrc;
    GetShaderBlocks(src, vertexSrc, fragSrc);

    _id = CreateShaderProgram(vertexSrc, fragSrc);
    LOG_DEBUG("Shader Program created: ID={}", _id);
}

Shader::~Shader()
{
    if (!_id)
        return;
    LOG_DEBUG("Destroying shader program: ID={}", _id);
    glDeleteProgram(_id);
    _id = 0;
}

void Shader::Bind() const
{
    glUseProgram(_id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetInt(const std::string& name, i32 value) const
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, f32 value) const
{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, f32 value[2]) const
{
    glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, value);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, f32 value[3]) const
{
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string& name, f32 value[4]) const
{
    glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, value);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

u32 Shader::CompileShader(const std::string& src, u32 type) const
{
    const char* srcPtr = src.c_str();
    u32         id     = glCreateShader(type);
    glShaderSource(id, 1, &srcPtr, nullptr);
    glCompileShader(id);

    i32 result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        i32 length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::string message(length, '\0');
        glGetShaderInfoLog(id, length, &length, message.data());
        glDeleteShader(id);
        throw ShaderException(type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", -1, "Shader compilation failed: " + message);
    }

    return id;
}

u32 Shader::CreateShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc) const
{
    u32 program = glCreateProgram();
    u32 vs      = CompileShader(vertexSrc, GL_VERTEX_SHADER);
    u32 fs      = CompileShader(fragmentSrc, GL_FRAGMENT_SHADER);

    if (!vs || !fs)
    {
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        glDeleteProgram(program);
        throw ShaderException("Program", -1, "Failed to compile shaders for program");
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    i32 linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        i32 length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string message(length, '\0');
        glGetProgramInfoLog(program, length, &length, message.data());
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        throw ShaderException("Program", -1, "Shader program linking failed: " + message);
    }


    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

std::string Shader::ReadFile() const
{
    const std::ifstream in(_filePath, std::ios::in | std::ios::binary);
    if (!in.is_open())
    {
        throw IOException("Failed to open shader file: " + _filePath);
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

i32 Shader::GetUniformLocation(const std::string& name)
{
    auto it = _uniformLookup.find(name);
    if (it != _uniformLookup.end())
        return it->second;
    i32 location = glGetUniformLocation(_id, name.c_str());
    if (location == -1)
        LOG_WARN("Shader uniform '{}' not found or was optimized out in shader '{}'", name, _filePath);

    _uniformLookup[name] = location;
    return location;
}

} // namespace stick