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

namespace stick
{

class EntityRegistry
{
public:
    entity_id CreateEntity()
    {
        AddComponent<Transform>(_nextId);
        _entities.push_back(_nextId);
        return _nextId++;
    }

    template<typename T, typename... Args>
    void AddComponent(entity_id entity, Args&&... args)
    {
        ComponentStorage<T>& storage = Storage<T>();

        if (storage.HasAny(entity))
        {
            throw SystemException("Entity {} already has component of type {}", entity, typeid(T).name());
        }

        storage.Add(entity, T(std::forward<Args>(args)...));
    }

    template<typename T>
    bool HasComponent(entity_id entity)
    {
        return Storage<T>().HasAny(entity);
    }

    template<typename T>
    T& GetComponent(entity_id entity)
    {
        return Storage<T>().Get(entity);
    }

    template<typename T>
    const T& GetComponent(entity_id entity) const
    {
        return Storage<T>().Get(entity);
    }

    Transform& GetTransform(const entity_id entity) { return GetComponent<Transform>(entity); }

    template<typename T>
    ComponentStorage<T>& Storage()
    {
        static ComponentStorage<T> storage;
        return storage;
    }

    template<typename T>
    const ComponentStorage<T>& Storage() const
    {
        return const_cast<EntityRegistry*>(this)->Storage<T>();
    }

    [[nodiscard]] const std::vector<entity_id>& Entities() const { return _entities; }

    template<typename... Components>
    [[nodiscard]] std::vector<entity_id> GetEntitiesWithComponents() const
    {
        std::vector<entity_id> entities;

        for (const entity_id entity : _entities)
        {
            if ((Storage<Components>().HasAny(entity) && ...))
            {
                entities.push_back(entity);
            }
        }

        return entities;
    }

private:
    entity_id _nextId = 1;

    std::vector<entity_id> _entities;
};

} // namespace stick