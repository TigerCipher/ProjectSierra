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
#include "Component.h"

#include <ranges>
#include <unordered_map>
#include <typeindex>
#include <type_traits>

namespace stick
{
class Entity
{
public:
    template<typename T, typename... Args> 
    T& AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<Component, T>, "T must be a derived type of Component");
        auto component         = CreateScope<T>(std::forward<Args>(args)...);
        component->SetOwner(this);
        T&   ref               = *component;
        _components[typeid(T)] = std::move(component);
        return ref;
    }

    template<typename T>
    T* GetComponent()
    {
        if (const auto it = _components.find(typeid(T)); it != _components.end())
        {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    [[nodiscard]] bool HasComponent() const
    {
        return _components.contains(typeid(T));
    }

    template<typename T>
    void RemoveComponent()
    {
        _components.erase(typeid(T));
    }

    void Update(const f32 delta)
    {
        for (const auto& comp : _components | std::views::values)
        {
            comp->Update(delta);
        }
    }

    void Render(Renderer& renderer)
    {
        for (const auto& comp : _components | std::views::values)
        {
            comp->Render(renderer);
        }
    }

private:
    std::unordered_map<std::type_index, scope<Component>> _components{};
};
} // namespace stick
