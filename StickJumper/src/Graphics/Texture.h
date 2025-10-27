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

namespace stick
{
class Texture
{
public:
    Texture(std::string path, bool flipVertical = true);
    virtual ~Texture();

    void Bind(u32 slot = 0) const;
    void Unbind() const;

    [[nodiscard]] constexpr u32 Id() const { return _id; }
    [[nodiscard]] constexpr u32 Width() const { return _width; }
    [[nodiscard]] constexpr u32 Height() const { return _height; }

    [[nodiscard]] constexpr i32 Channels() const { return _channels; }

private:
    u32         _id       = 0;
    i32         _width    = 0;
    i32         _height   = 0;
    i32         _channels = 0;
    std::string _path;
};
} // namespace stick