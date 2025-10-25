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

#include "SettingsManager.h"
#include <fstream>
#include <string>

namespace stick
{

void SettingsManager::Load()
{
    if (!std::filesystem::exists(_path))
    {
        create_directories(_path.parent_path());
        LOG_WARN("Settings file not found at {}. Creating with default values.", absolute(_path).string());
        // Create container with defaults
        _root = json::object();
        for (auto& [key, value] : _defaults.items())
        {
            _root[key] = value;
        }
        Save();
        return;
    }

    std::ifstream in(_path);
    if (!in.is_open())
    {
        throw IOException(std::format("Unable to open file {}", absolute(_path).string()));
    }

    in >> _root;

    // Make sure defaults exist
    for (auto& [key, value] : _defaults.items())
    {
        if (!_root.contains(key))
        {
            _root[key] = value;
        }
    }

    if (_root.contains("version") && _root["version"].is_number_integer())
        _version = _root["version"].get<i32>();
}

void SettingsManager::Save() const
{
    json out = _root;
    out["version"] = _version;

    std::ofstream ofs(_path);
    LOG_INFO("Saving settings to {}", absolute(_path).string());
    if (!ofs.is_open())
    {
        throw IOException(std::format("Unable to open file {}", absolute(_path).string()));
    }

    ofs << out.dump(4);
}

// Explicit template instantiations for commonly used types (avoid linker errors)
template int SettingsManager::Get<i32>(const std::string&) const;
template float SettingsManager::Get<f32>(const std::string&) const;
template double SettingsManager::Get<f64>(const std::string&) const;
template bool SettingsManager::Get<bool>(const std::string&) const;
template std::string SettingsManager::Get<std::string>(const std::string&) const;

template std::optional<i32> SettingsManager::TryGet<i32>(const std::string&) const;
template std::optional<f32> SettingsManager::TryGet<f32>(const std::string&) const;
template std::optional<f64> SettingsManager::TryGet<f64>(const std::string&) const;
template std::optional<bool> SettingsManager::TryGet<bool>(const std::string&) const;
template std::optional<std::string> SettingsManager::TryGet<std::string>(const std::string&) const;

template void SettingsManager::Set<i32>(const std::string&, const i32&);
template void SettingsManager::Set<f32>(const std::string&, const f32&);
template void SettingsManager::Set<f64>(const std::string&, const f64&);
template void SettingsManager::Set<bool>(const std::string&, const bool&);
template void SettingsManager::Set<std::string>(const std::string&, const std::string&);

} // namespace stick