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
#include <string>
#include <filesystem>
#include <optional>

#include <nlohmann/json.hpp>


namespace stick
{
using json = nlohmann::json;

class SettingsManager
{
public:
    explicit SettingsManager(std::filesystem::path path) : _path(std::move(path)) {}

    void Load();
    void Save() const;

    // Register a default value for a key (will be set if missing on load)
    void RegisterDefault(const std::string& key, const json& value) { _defaults[key] = value; }

    // Generic get/set. If key missing and default was registered, return default.
    template<typename T>
    T Get(const std::string& key) const
    {
        auto it = _root.find(key);
        if (it != _root.end())
        {
            LOG_DEBUG("Value found for key {}", key);
            return it->get<T>();
        }

        LOG_DEBUG("Key not found, reverting to defaults for {}", key);
        auto d = _defaults.find(key);
        if (d != _defaults.end())
        {
            return d->get<T>();
        }

        throw StickException(std::format("Settings key not found: {}", key));
    }

    template<typename T>
    std::optional<T> TryGet(const std::string& key) const
    {
        auto it = _root.find(key);
        if (it != _root.end())
        {
            try
            {
                return it->get<T>();
            } catch (...)
            {
                return std::nullopt;
            }
        }
        auto d = _defaults.find(key);
        if (d != _defaults.end())
        {
            try
            {
                return d->get<T>();
            } catch (...)
            {
                return std::nullopt;
            }
        }
        return std::nullopt;
    }

    template<typename T>
    void Set(const std::string& key, const T& value)
    {
        _root[key] = value;
    }

    [[nodiscard]] bool Has(const std::string& key) const { return _root.contains(key); }

    [[nodiscard]] const json& Root() const { return _root; }
    json&                     Root() { return _root; }

    void              SetVersion(const i32 version) { _version = version; }
    [[nodiscard]] i32 Version() const { return _version; }

private:
    std::filesystem::path _path;
    json                  _root;
    json                  _defaults;
    i32                   _version = 0;
};

} // namespace stick
