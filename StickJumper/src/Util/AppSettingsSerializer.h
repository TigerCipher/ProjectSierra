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

#include "Core/AppSettings.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

inline void to_json(json& j, const AppSettings& settings)
{
    j = json{
            {"LimitFrameRate", settings.LimitFrameRate},
            {"TargetFrameRate", settings.TargetFrameRate},
            {"UseVsync", settings.UseVsync}
    };
}

inline void from_json(const json& j, AppSettings& settings)
{
    if (j.contains("LimitFrameRate")) settings.LimitFrameRate = j.at("LimitFrameRate").get<bool>();
    if (j.contains("TargetFrameRate")) settings.TargetFrameRate = j.at("TargetFrameRate").get<float>();
    if (j.contains("UseVsync")) settings.UseVsync = j.at("UseVsync").get<bool>();
}

