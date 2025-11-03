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

#include "Component.h"

#include "Entity.h"

namespace stick
{

template<typename T>
class ComponentStorage
{
public:
    void Add(entity_id entity, const T& component) { _components[entity] = component; }

    [[nodiscard]] bool HasAny(entity_id entity) const { return _components.contains(entity); }

    T& Get(entity_id entity) { return _components.at(entity); }

    [[nodiscard]] const T& Get(entity_id entity) const { return _components.at(entity); }

    void Remove(entity_id entity) { _components.erase(entity); }

private:
    std::unordered_map<entity_id, T> _components;
};

} // namespace stick