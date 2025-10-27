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

#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace stick
{

Texture::Texture(std::string path, bool flipVertical) : _path(std::move(path))
{
    LOG_INFO("Loading texture from file: {}", _path);
    stbi_set_flip_vertically_on_load(flipVertical);

    u8* data = stbi_load(_path.c_str(),
        &_width,
        &_height,
        &_channels, 0);

    if (!data)
    {
        throw IOException("Failed to load texture from file '" + _path + "'");
    }

    GLint internalFormat = 0;
    GLenum dataFormat = 0;
    if (_channels == 4)
    {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }else if (_channels == 3)
    {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }else
    {
        stbi_image_free(data);
        throw GfxException("Unsupported texture format with " + std::to_string(_channels) + " channels: " + _path);
    }

    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    LOG_INFO("Texture loaded successfully: {} ({}x{}, {} channels)", _path, _width, _height, _channels);
}

Texture::~Texture()
{
    LOG_DEBUG("Texture destroyed: ID={}, Path={}", _id, _path);
    glDeleteTextures(1, &_id);
}

void Texture::Bind(const u32 slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

} // namespace stick