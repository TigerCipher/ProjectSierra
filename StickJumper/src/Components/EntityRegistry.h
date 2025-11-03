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
#include "ComponentStorage.h"
#include "ComponentId.h"
#include "Entity.h"

namespace stick
{

class EntityRegistry
{
public:
    Entity& CreateEntity(const Transform& transform = Transform())
    {
        auto& newEntity = _entities.emplace_back(_nextId++);
        AddComponent<Transform>(newEntity, transform);
        return newEntity;
    }

    template<typename T, typename... Args>
    void AddComponent(Entity& entity, Args&&... args)
    {
        if (entity.HasComponent<T>())
        {
            throw SystemException("Entity {} already has component of type {}", entity.Id, typeid(T).name());
        }

        ComponentStorage<T>& storage = Storage<T>();
        storage.Add(entity.Id, T(std::forward<Args>(args)...));
        entity.AddComponent<T>();
    }

    template<typename T>
    bool HasComponent(entity_id entityId)
    {
        return Storage<T>().HasAny(entityId);
    }

    template<typename T>
    T& GetComponent(const Entity& entity)
    {
        return Storage<T>().Get(entity.Id);
    }

    template<typename T>
    [[nodiscard]] const T& GetComponent(const Entity& entity) const
    {
        return Storage<T>().Get(entity.Id);
    }

    template<typename... Components, typename Func>
    void ForEach(Func&& func)
    {
        component_mask requiredMask = CreateComponentMask<Components...>();

        for (auto& entity : _entities)
        {
            if (!entity.HasComponents<Components...>())
                continue;

            func(entity, GetComponent<Components>(entity)...);
        }
    }

    template<typename... Components, typename Func>
    void ForEach(Func&& func) const
    {
        component_mask requiredMask = CreateComponentMask<Components...>();

        for (const auto& entity : _entities)
        {
            if (!entity.HasComponents<Components...>())
                continue;

            func(entity, GetComponent<Components>(entity)...);
        }
    }

    Transform& GetTransform(const Entity& entity) { return GetComponent<Transform>(entity); }

    template<typename T>
    ComponentStorage<T>& Storage()
    {
        static ComponentStorage<T> storage;
        return storage;
    }

    template<typename T>
    [[nodiscard]] const ComponentStorage<T>& Storage() const
    {
        return const_cast<EntityRegistry*>(this)->Storage<T>();
    }

    [[nodiscard]] const std::vector<Entity>& Entities() const { return _entities; }


private:
    entity_id _nextId = 1;

    std::vector<Entity> _entities;
};

} // namespace stick