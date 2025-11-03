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
#include "ComponentId.h"

#ifdef STICK_ENTITY_SIZE
using entity_id = STICK_ENTITY_SIZE;
#else
using entity_id = u32;
#endif

namespace stick
{

struct Entity
{
    entity_id      Id;
    component_mask Mask = 0;

    explicit Entity(const entity_id id) : Id(id) {}

    template<typename T>
    void AddComponent()
    {
        const component_id id = GetComponentTypeId<T>();
        Mask |= (1 << id);
    }

    template<typename T>
    void RemoveComponent()
    {
        const component_id id = GetComponentTypeId<T>();
        Mask &= ~(1 << id);
    }

    template<typename T>
    [[nodiscard]] bool HasComponent() const
    {
        const component_id id = GetComponentTypeId<T>();
        return (Mask & (1 << id)) != 0;
    }

    template<typename... Components>
    [[nodiscard]] bool HasComponents() const
    {
        const component_mask requiredMask = CreateComponentMask<Components...>();
        return (Mask & requiredMask) == requiredMask;
    }
};

} // namespace stick